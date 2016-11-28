#include "base.h"
#include "server.h"


#define MAXBUF 4096
int calculateNums = 0;
std::map<int,fdBuf_t*> fdMap;
int main(int argc, char const *argv[])
{

	int port;
	int serversock = -1;

	if (argc < 2){
		printf("arg is too less\n");
		return 0;
	}

	port=atoi(argv[1]);
	serversock=socketinit(port);
	if (serversock == -1)
	{
		perror("socketinit");
		return 0;
	}

	addCalculate();


	struct event_base* base =  event_base_new();

	struct event* evListen = event_new(base,serversock,EV_READ | EV_PERSIST,acceptCb,(void*)base);
	event_add(evListen,NULL);

	event_base_dispatch(base);
	event_free(evListen);
	event_base_free(base);


	return 0;
}


void acceptCb(int fd, short events, void* arg)
{
	int clientFd;
	clientFd = accept(fd,NULL,NULL);
	if(clientFd < 0){
		perror("accept");
		return;
	}
	printf("accept a client %d\n", clientFd);
	if(addMap(clientFd) < 0){
		perror("addMap");
		return;
	}

	evutil_make_socket_nonblocking(clientFd);
	struct event_base* base  = (struct event_base*)arg;

	bufferevent *bev = bufferevent_socket_new(base,clientFd,BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev,readCb,NULL,eventCb,arg);
	bufferevent_enable(bev,EV_READ|EV_PERSIST);

	return ;
}

void readCb(bufferevent*bev ,void* arg)
{
	struct event *ev = (struct event*)arg;
	int recvLen = 0;
	int fd = bufferevent_getfd(bev);
	fdBuf_t *tempfd = findMap(fd);
	
	if(tempfd == NULL){
		perror("findMap");
		return ;
	}
	printf("-----------%d\n", tempfd->maxLen - ( tempfd->bufPoint - tempfd->buf) - tempfd->bufLen);

	recvLen = bufferevent_read(bev,tempfd->bufPoint + tempfd->bufLen,tempfd->maxLen - ( tempfd->bufPoint - tempfd->buf) - tempfd->bufLen);

	tempfd->bufLen += recvLen;
	dealBuf(tempfd);

}

void eventCb(bufferevent*bev ,short event,void* arg)
{
	int fd = bufferevent_getfd(bev);
	
    if (event & BEV_EVENT_EOF)  
        printf("connection closed\n");  
    else if (event & BEV_EVENT_ERROR)  
        printf("some other error\n");  
  	
  	if(removeMap(fd) < 0){
			perror("removeMap");
	}
    //这将自动close套接字和free读写缓冲区  
    bufferevent_free(bev);
}

















int addMap(int fd)
{
	fdBuf_t *temp = (fdBuf_t*)malloc(sizeof(fdBuf_t));
	if (NULL ==  temp){
		perror("malloc");
		return -1;
	}
	temp->fd=fd;
	temp->buf=(char *)malloc(MAXBUF);
	if (NULL ==  temp->buf){
		free(temp);
		perror("malloc");
		return -1;
	}
	temp->bufPoint = temp->buf;
	temp->bufLen = 0;
	temp->maxLen = MAXBUF;
	std::pair<std::map<int,fdBuf_t*>::iterator,bool> ret;
	ret = fdMap.insert(std::pair<int,fdBuf_t*>(fd,temp));
	if (ret.second == false){
		free(temp->buf);
		free(temp);
		perror("insert");
		return -1;
	}

	return 0;

}

int removeMap(int fd)
{
	std::map<int,fdBuf_t*>::iterator it;
	it = fdMap.find(fd);
	if (it != fdMap.end()){
		free(it->second->buf);
		free(it->second);
		fdMap.erase(it);
		return 0;
	}
	return -1;
}

fdBuf_t* findMap(int fd)
{
	std::map<int,fdBuf_t*>::iterator it;
	it = fdMap.find(fd);
	if (it == fdMap.end()){
		return NULL;
	}
	return it->second;
}



int dealBuf(fdBuf_t * fdBuf)
{
	static char tempBuf[1500];

	if ( 	(fdBuf->bufPoint != fdBuf->buf) &&
			(fdBuf->bufPoint + fdBuf->bufLen) == (fdBuf->buf + fdBuf->maxLen)
		){
		memcpy(fdBuf->buf ,fdBuf->bufPoint,fdBuf->bufLen);
		fdBuf->bufPoint = fdBuf->buf;
	}

	int headSize = sizeof(head_t);

	for (;;){
		if (0 == fdBuf->bufLen){		
			return 0;
		}
		if (headSize > fdBuf->bufLen){
			printf("headSize:%d-------bufLen:%d-----------半包\n",headSize,fdBuf->bufLen);
			return -1;
		}
		head_t *head = (head_t*)(fdBuf->bufPoint);
		int cmd = head->cmd;
		if (cmd != LOGIN && cmd != READY && cmd != MSG && cmd != LEAVE ){
			fdBuf->bufPoint += 1;
			fdBuf->bufLen -= 1;
			printf("head->size:%d-------bufLen:%d-----------乱包\n",head->size,fdBuf->bufLen);
			continue;
		}
		if(head->size > fdBuf->bufLen){
			printf("head->size:%d-------bufLen:%d-----------半包\n",head->size,fdBuf->bufLen);
			return -1;
		}

		
		memcpy(tempBuf,fdBuf->bufPoint,head->size);

		switch(cmd){
			case LOGIN:	dealBufLogin(tempBuf,head->size);
				break;
			case READY:	dealBufReady(tempBuf,head->size);
				break;
			case MSG:	dealBufMsg(tempBuf,head->size);
				break;
			case LEAVE:	dealBufLeave(tempBuf,head->size);
				break;
			default:
				break;
		}
		if (fdBuf->bufLen > head->size){
			printf("head->size:%d-------bufLen:%d-----------粘包\n",head->size,fdBuf->bufLen);
		}
		fdBuf->bufPoint += head->size;
		fdBuf->bufLen -= head->size;

		++calculateNums;
	}

	return 0;
}

int dealBufLogin(const char *buf,const int bufLen)
{
	login_t *que = (login_t*)(buf);
	printf("LOGIN:useid:%d,token:%s\n",que->userid,que->token);
}

int dealBufReady(const char *buf,const int bufLen)
{
	ready_t *que = (ready_t*)(buf);
	printf("READY:useid:%d\n",que->userid);
}

int dealBufMsg(const char *buf,const int bufLen)
{
	msg_t *que = (msg_t*)(buf);
	printf("MSG:useid:%d,",que->userid);
	write(STDOUT_FILENO,que->data,que->dataSize);
	printf("\n");

}

int dealBufLeave(const char *buf,const int bufLen)
{
	leave_t *que = (leave_t*)(buf);
	printf("LEAVE:useid:%d,score:%d\n",que->userid,que->score);
}


int socketinit(int port)
{
	int tempSock = socket(AF_INET,SOCK_STREAM,0);
	if(tempSock == -1){
		perror("init socket");
		return -1;
	}
	struct sockaddr_in serverAddr;
	bzero(&serverAddr,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr  = htonl(INADDR_ANY);
	int flag=1;
	if (setsockopt(tempSock,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag)) < 0){
		perror("setsockopt");
	}

	if(bind(tempSock,(struct sockaddr *)&serverAddr,sizeof(struct sockaddr)) == -1){
		perror("bind");
		return -1;
	}
	if (listen(tempSock,128) == -1)
	{
		perror("listen");
		return -1;
	}
	return tempSock;

}

void addCalculate()
{
	pthread_attr_t attr;
	pthread_t threadId;
	int preturn;

	if (pthread_attr_init(&attr)){
		perror("pthread_attr_init");
		return;
	}
	if (pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED)){
		perror("pthread_attr_setdetachstate");
		return ;
	}
	preturn = pthread_create(&threadId,&attr,calculate,NULL);
	if (preturn != 0){
		perror("pthread_create");
	}
}

void* calculate(void *param)
{
	while(1){
		sleep(1);
		printf("-----------------------------------------------------------%d\n",calculateNums );
		//calculateNums = 0;
	}
	return NULL;
}