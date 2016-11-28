#include "base.h"
#include "server.h"


#define MAXBUF 1500
#define EPOLL_SIZE 2
#define PTHREAD_EPOLL_SIZE 2

int calculateNums = 0;
int main(int argc, char const *argv[])
{

	int port;
	char buf[MAXBUF];
	int online = 0;
	std::list<int> fdList;
	int serversock = -1;
	std::deque<int> clientQueue;
	CommonMutex lock;
	pthread_cond_t has_product;

	mysqlPool::getInstance();

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
	pthread_attr_t attr;
	if (pthread_attr_init(&attr)){
		perror("pthread_attr_init");
		return 0;
	}
	if (pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED)){
		perror("pthread_attr_setdetachstate");
		return 0;
	}

///------------------------------------------------回环连表
	controlUnit *first = NULL,*pre=NULL,*controlGo = NULL;
	for ( int i= 0; i < 4 ; ++i){
		pre = controlGo;
		controlGo = new controlUnit();
		if( NULL == first ){
			first = controlGo;
		}
		controlGo->setNext(pre);

		pthread_t threadId;
		int preturn;
		preturn = pthread_create(&threadId,&attr,deal,(void*)controlGo);
		if (preturn != 0){
			perror("pthread_create");
			continue;
		}
	}
	first->setNext(controlGo);
//----------------------------------------------------------
	pthread_t threadId;
	pthread_create(&threadId,&attr,calculate,NULL);

	int epollFd = epoll_create(EPOLL_SIZE);
	if(epollFd == -1){
		perror("epoll create");
		close(serversock);
		return 0;
	}
	struct epoll_event serverEpollEvent,readyEvent[EPOLL_SIZE];
	serverEpollEvent.events = EPOLLIN;
	serverEpollEvent.data.fd = serversock;
	if (epoll_ctl(epollFd,EPOLL_CTL_ADD,serversock,&serverEpollEvent) == -1){
		perror("epoll add serversock");
		close(serversock);
		return 0;
	}

	int acceptClient = 0;
	int readyEventNums = 0 ;
	char bufFd[10];
	for ( ;  ; ){
		readyEventNums = epoll_wait(epollFd,readyEvent,EPOLL_SIZE,0);
		if(readyEventNums > 0)
		{
			if (!(readyEvent[0].events & EPOLLIN)){
				continue;
			}
			//add 和 cond
			int acceptClient = accept(serversock,NULL,NULL);
			if (acceptClient == -1){
				perror("accept client");
				continue;
			}	
			controlGo = controlGo->getNext();
			sprintf(bufFd,"%d_",acceptClient);
			write(controlGo->getWritePipe(),bufFd,strlen(bufFd));
		}else{
			
		}
	}

	close(serversock);
	return 0;
}


void* deal(void *param)
{
	controlUnit *cont = (controlUnit*)param;
	MYSQL* sqlConn = NULL;
	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
	char buf[100];
	int readPipe = cont->getReadPipe();


	int pthreadEpollFd = epoll_create(PTHREAD_EPOLL_SIZE);
	if(pthreadEpollFd == -1){
		perror("pthreadEpollFd create");
		return 0;
	}
	struct epoll_event pthreadEpollEvent,readyEvent[PTHREAD_EPOLL_SIZE];
	pthreadEpollEvent.events = EPOLLIN;
	pthreadEpollEvent.data.fd = readPipe;
	if (epoll_ctl(pthreadEpollFd,EPOLL_CTL_ADD,readPipe,&pthreadEpollEvent) == -1){
		perror("epoll add serversock");
		return 0;
	}

	int readyEventNums = 0 ;
	int const bufSize = 512;
	char bufFd[bufSize];
	int arrFd[102400];
	int arrNums = 0;
	int readLen = 0;
	for ( ;  ; ){
		
		readyEventNums = epoll_wait(pthreadEpollFd,readyEvent,PTHREAD_EPOLL_SIZE,0);
		//write(STDOUT_FILENO,"ee",strlen("ee"));
		for (int i = 0; i < readyEventNums; ++i){
			if (!(readyEvent[i].events & EPOLLIN)){			
				continue;
			}

			if (readyEvent[i].data.fd == readPipe){

				if( (readLen = read(readPipe,bufFd,bufSize - 1)) < 0){
					perror("read");
				}else{
					bufFd[readLen] = '\0';
					write(STDOUT_FILENO,bufFd,strlen(bufFd));
					write(STDOUT_FILENO,"\n",strlen("\n"));
					getNewClientArr(bufFd,'_',arrFd,&arrNums);
					for (int i = 0; i < arrNums; ++i)
					{
						cont->lock();
						cont->push_back(arrFd[i]);
						cont->unlock();
					}
				}
			}else{

			}
		}
		if ((sqlConn= mysqlPool::getInstance()->getConn()) != NULL ){
			arrNums = 102400;
			cont->lock();
			cont->getArrFd(arrFd,&arrNums);
			cont->unlock();
			for (int i = 0; i < arrNums; ++i)
			{
				const char * query= "select gameID,productName from test.product_info";
				int re = mysql_query(sqlConn,query);
				if (re == 0){
					res = mysql_store_result(sqlConn);
					while(row = mysql_fetch_row(res)){
						sprintf(buf,"gameID:%s,productName:%s\n",row[0],row[1]);
						if((sendMsg(arrFd[i],buf,strlen(buf)))<0){
							cont->lock();
							close(arrFd[i]);
							cont->remove(arrFd[i]);
							cont->unlock();
						}
					}			
					mysql_free_result(res);
				}
				++calculateNums;
			}
			mysqlPool::getInstance()->recycConn(sqlConn);
		}	
	}

}

int getNewClientArr(const char *buf,const char sep,int *arrFd,int *arrNums)
{
	static char leaveBuf[10];
	char allBuf[1024]={0};
	strcat(allBuf,leaveBuf);
	strcat(allBuf,buf);
	int len = strlen(allBuf);
	int i=0;
	int j=0;
	int k=0;
	for(i;i<len;++i){
		if(allBuf[i] == sep){
			leaveBuf[j] = '\0';
			arrFd[k] = atoi(leaveBuf);
			++k;
			j = 0;
			continue;
		}
		leaveBuf[j] = allBuf[i];
		++j;		
	}
	leaveBuf[j] = '\0';
	*arrNums = k;
	return 0;
}


int sendMsg(int originFd,const char *msg,int msgLen)
{
	return  send(originFd,msg,msgLen,0);
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

void* calculate(void *param)
{
	while(1){
		sleep(1);
		printf("%d\n",calculateNums );
		calculateNums = 0;
	}
	return NULL;
}