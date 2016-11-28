#include "base.h"
#include "client.h"
#include <time.h>

#define MAXBUF 1500
#define EPOLL_SIZE 2

int main(int argc, char const *argv[])
{
	char ip[32] = {0};
	int port;
	char buf[MAXBUF];
	char stdinBuf[MAXBUF];
	int sockLink = -1;

	if (argc < 3){
		printf("arg is too less\n");
		return 0;
	}

	strcpy(ip,argv[1]);
	port  = atoi(argv[2]);

	sockLink = connectServer(ip,port);
	if (sockLink == -1 ){
		return 0;
	}
	int epollFd = epoll_create(EPOLL_SIZE);
	if(epollFd == -1){
		perror("epoll create");
		close(sockLink);
		return 0;
	}
	struct epoll_event serverEpollEvent,stdinEpollEvent,readyEvent[EPOLL_SIZE];
	serverEpollEvent.events = EPOLLIN;
	serverEpollEvent.data.fd = sockLink;
	stdinEpollEvent.events = EPOLLIN;
	stdinEpollEvent.data.fd = STDIN_FILENO;
	if (epoll_ctl(epollFd,EPOLL_CTL_ADD,sockLink,&serverEpollEvent) == -1){
		perror("epoll add sockLink");
		close(sockLink);
		return 0;
	}	
	if (epoll_ctl(epollFd,EPOLL_CTL_ADD,STDIN_FILENO,&stdinEpollEvent) == -1){
		perror("epoll add STDIN_FILENO");
		close(sockLink);
		return 0;
	}

	int readyEventNums = 0 ;
	for ( ; ; )
	{
		readyEventNums = epoll_wait(epollFd,readyEvent,EPOLL_SIZE,-1);
		for (int i = 0; i < readyEventNums; ++i)
		{
			if (!(readyEvent[i].events & EPOLLIN)){
				continue;
			}
			if (readyEvent[i].data.fd == sockLink ){
				int recvLen = recv(readyEvent[i].data.fd,buf,MAXBUF-1,0);
				if(recvLen > 0){
					write(STDOUT_FILENO,buf,recvLen);

				}else if (recvLen == 0){
					close(readyEvent[i].data.fd);
					epoll_ctl(epollFd,EPOLL_CTL_DEL,readyEvent[i].data.fd,NULL);
					printf("client is close");
					return 0;

				}else{
					perror("a client have error");
					return 0;
				}
			}
			else{
				int readLen = read(STDIN_FILENO,stdinBuf,MAXBUF);
				if(readLen > 0){
					int sendLen = send(sockLink,stdinBuf,readLen,0);
					if (sendLen < 0){
						perror("sockLink have error");
						return 0;
					}

				}else if (readLen == 0){
					close(readyEvent[i].data.fd);
					epoll_ctl(epollFd,EPOLL_CTL_DEL,readyEvent[i].data.fd,NULL);
					printf("STDIN_FILENO is close");
					return 0;

				}else{
					perror("STDIN_FILENO have error");
					return 0;
				}
			}
		}
	}

	close(sockLink);
	return 0;
}


int connectServer(char *ip,int port)
{
	int clientSoct = socket(AF_INET,SOCK_STREAM,0);
	if (clientSoct == -1){
		perror("init socket");
		return -1;
		/* code */
	}

	struct sockaddr_in clientSockAddr;
	bzero(&clientSockAddr,sizeof(clientSockAddr));
	clientSockAddr.sin_family=AF_INET;
	clientSockAddr.sin_port = htons(port);
	clientSockAddr.sin_addr.s_addr = inet_addr(ip);

	if (connect(clientSoct,(struct sockaddr *)&clientSockAddr,sizeof(struct sockaddr)) == -1){
		perror("connect socket");
		return -1;
	}
	return clientSoct;
}