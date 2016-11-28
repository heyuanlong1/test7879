#include "base.h"
#include "server.h"


#define MAXBUF 4096
#define EPOLL_SIZE 1024

std::map<int,fdBuf_t*> fdMap;
int main(int argc, char const *argv[])
{

	int port;
	int online = 0;
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
	int epollFd = epoll_create(EPOLL_SIZE);
	if(epollFd == -1){
		perror("epoll create");
		close(serversock);
		return 0;
	}

	struct epoll_event serverEpollEvent,tempEvent,readyEvent[EPOLL_SIZE];
	serverEpollEvent.events = EPOLLIN;
	serverEpollEvent.data.fd = serversock;
	if (epoll_ctl(epollFd,EPOLL_CTL_ADD,serversock,&serverEpollEvent) == -1){
		perror("epoll add serversock");
		close(serversock);
		return 0;
	}
	++online;

	int readyEventNums = 0 ;
	int res=0;
	for ( ; ; )
	{
		readyEventNums = epoll_wait(epollFd,readyEvent,EPOLL_SIZE,-1);
		for (int i = 0; i < readyEventNums; ++i)
		{
			if (!(readyEvent[i].events & EPOLLIN)){
				continue;
			}
			if (readyEvent[i].data.fd == serversock ){
				int acceptClient = accept(serversock,NULL,NULL);
				if (acceptClient == -1){
					perror("accept client");
					continue;
				}
				printf("a new client:%d\n",acceptClient);

				if(addMap(acceptClient) < 0){
					perror("addMap");
					continue;
				}

				tempEvent.events = EPOLLIN;
				tempEvent.data.fd = acceptClient;
				if ((online+1) > EPOLL_SIZE){
					printf("more than EPOLL_SIZE\n");
					continue;
				}
				if (epoll_ctl(epollFd,EPOLL_CTL_ADD,acceptClient,&tempEvent) == -1){
					perror("epoll add acceptClient");
					continue;
				}
				++online;
			}else{

				fdBuf_t *tempfd = findMap(readyEvent[i].data.fd);
				if(tempfd == NULL){
					perror("findMap");
					continue;
				}
				printf("-----------%d\n", tempfd->maxLen - ( tempfd->bufPoint - tempfd->buf) - tempfd->bufLen);
				int recvLen = recv(readyEvent[i].data.fd,tempfd->bufPoint + tempfd->bufLen,tempfd->maxLen - ( tempfd->bufPoint - tempfd->buf) - tempfd->bufLen,0);
				if(recvLen > 0){
					tempfd->bufLen += recvLen;
					dealBuf(tempfd);
				}else if (recvLen == 0){
					perror("recvLen == 0");
					if(removeMap(readyEvent[i].data.fd) < 0){
						perror("removeMap");
					}
					//close(readyEvent[i].data.fd); // 听说不用释放
					epoll_ctl(epollFd,EPOLL_CTL_DEL,readyEvent[i].data.fd,NULL);
					--online;
					continue;		
				}else{
					if(removeMap(readyEvent[i].data.fd) < 0){
						perror("removeMap");
					}
					epoll_ctl(epollFd,EPOLL_CTL_DEL,readyEvent[i].data.fd,NULL);
					--online;

					close(readyEvent[i].data.fd);
					perror("a client have error");	
					continue;			
				}
				
			}
		}
	}
	return 0;
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

	if ( (fdBuf->bufPoint + fdBuf->bufLen) == (fdBuf->buf + fdBuf->maxLen) ){
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