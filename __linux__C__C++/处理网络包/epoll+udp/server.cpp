#include "base.h"
#include "server.h"


#define MAXBUF 1500
#define EPOLL_SIZE 1024

int main(int argc, char const *argv[])
{

	int port;
	char buf[MAXBUF];
	int online = 0;
	std::list<int> fdList;
	int serversock = -1;

	if (argc < 2){
		printf("arg is too less\n");
		return 0;
	}

	port  = atoi(argv[1]);

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
	for ( ; ; )
	{
		readyEventNums = epoll_wait(epollFd,readyEvent,EPOLL_SIZE,-1);
		for (int i = 0; i < readyEventNums; ++i)
		{
			if (!(readyEvent[i].events & EPOLLIN)){
				continue;
			}
			if (readyEvent[i].data.fd == serversock ){
				deal(serversock);
			}
		}
	}
	return 0;
}


int socketinit(int port)
{
	int tempSock = socket(AF_INET,SOCK_DGRAM,0);
	if(tempSock == -1){
		perror("init socket");
		return -1;
	}
	int opt=1;
  	setsockopt(tempSock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
  	setnonblocking(tempSock);

	struct sockaddr_in serverAddr;
	bzero(&serverAddr,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr  =  htonl(INADDR_ANY);

	if(bind(tempSock,(struct sockaddr *)&serverAddr,sizeof(struct sockaddr)) == -1){
		perror("bind");
		return -1;
	}
	return tempSock;

}
int setnonblocking(int sockfd)
{
  if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1)
  {
    return -1;
  }
  return 0;
}


void deal(int sock_fd)
{
  char recvbuf[MAXBUF + 1];
  char sendbuf[MAXBUF + 1];
  int  ret;
  int  new_fd;
  struct sockaddr_in client_addr;
  socklen_t cli_len=sizeof(client_addr);

  new_fd=sock_fd; 
  bzero(recvbuf, MAXBUF + 1);
  bzero(sendbuf, MAXBUF + 1);


  /* 接收客户端的消息 */
  ret = recvfrom(new_fd, recvbuf, MAXBUF, 0, (struct sockaddr *)&client_addr, &cli_len);
  if (ret > 0){
    	printf("socket %d recv from : %s : %d message: %s ，%d bytes\n",new_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), recvbuf, ret);


   		sprintf(sendbuf,"server info");
   		ret = sendto(new_fd, sendbuf, strlen(sendbuf), 0, (struct sockaddr *)&client_addr, cli_len);
   		if(ret<0){
    		printf("消息发送失败！错误代码是%d，错误信息是'%s'\n", errno, strerror(errno));
    	}
  }
  else{
    printf("received failed! error code %d，message : %s \n",
      errno, strerror(errno));    
  }
}