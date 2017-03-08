#include "base.h"
#include "server.h"


#define MAXBUF 1500
#define EPOLL_SIZE 1024

int main(int argc, char const *argv[])
{

	char ip[32] = {0};
	int port;
	char buf[MAXBUF];
	int online = 0;
	std::list<int> fdList;
	int serversock = -1;

	if (argc < 3){
		printf("arg is too less\n");
		return 0;
	}

	strcpy(ip,argv[1]);
	port  = atoi(argv[2]);

	serversock=socketinit(ip,port);
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
				addList(&fdList,acceptClient);
				++online;

			}else{
				int recvLen = recv(readyEvent[i].data.fd,buf,MAXBUF-1,0);
				if(recvLen > 0){
					sendMsg(&fdList,readyEvent[i].data.fd,buf,recvLen);

				}else if (recvLen == 0){
					close(readyEvent[i].data.fd);
					epoll_ctl(epollFd,EPOLL_CTL_DEL,readyEvent[i].data.fd,NULL);
					removeList(&fdList,readyEvent[i].data.fd);
					--online;

				}else{
					perror("a client have error");
				}

			}
		}
	}
	return 0;
}


int sendMsg(std::list<int> *li,int originFd,char *msg,int msgLen)
{
	int sendLen = 0;
	std::list<int>::iterator b,e;
	b = li->begin();
	e = li->end();
	for (; b != e; ++b){
		if (*b != originFd ){		
			write(STDOUT_FILENO,msg,msgLen);
			sendLen = send(*b,msg,msgLen,0);
			if(sendLen < 0){
				perror("send");
			}
		}
	}
}

int addList(std::list<int> *li,int fd)
{
	printf("addList:%d\n", fd);
	bool have =false;
	std::list<int>::iterator b,e;
	b = li->begin();
	e = li->end();
	for (; b != e; ++b){
		if (*b == fd ){
			have = true;
			break;
		}
	}
	if(have == false){
		li->push_back(fd);
	}
	return 0;
}

int removeList(std::list<int> *li,int fd)
{
	printf("removeList:%d\n", fd);
	li->remove(fd);
	return 0;
}

int socketinit(char *ip,int port)
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
	serverAddr.sin_addr.s_addr  =  inet_addr(ip);

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