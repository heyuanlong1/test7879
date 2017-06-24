#include "base.h"
#include "client.h"
#include <time.h>

#define MAXBUF 1500
int main(int argc, char const *argv[])
{
	
	char ip[20];
	strcpy(ip,argv[1]);
	int port=atoi(argv[2]);
	char stdinBuf[MAXBUF];
	char bufstr[MAXBUF];
	int clientsocket = socket(AF_INET,SOCK_DGRAM,0);
	if (-1 == clientsocket){
		perror("client socket");
		exit(1);
	}

	//设置为异步
	setNotbolck(clientsocket);

	while(1){
		int readLen = read(STDIN_FILENO,stdinBuf,MAXBUF);
		if(readLen > 0){
			//stdinBuf[readLen] = '\0';
			int snum = udpSend(clientsocket,ip,port,stdinBuf,readLen - 1,0);
			if (snum < 0 )
			{
				perror("err:");
			}
		}
		else{
			printf("nothing\n");
		}

		int rnum = udpRecv(clientsocket,bufstr,sizeof(bufstr)-1,0);
		if(rnum > 0){
			bufstr[rnum] = '\0';
			printf("%s\n", bufstr);
		}
	}


	return 0;
}




int setNotbolck(int sockfd)
{
	return fcntl(sockfd,F_SETFL,fcntl(sockfd,F_GETFL,0) | O_NONBLOCK);
}
int udpSend(int sockfd,const char *ip,const int port,const void *buf,const int len,int flags)
{
	struct sockaddr_in addr;
	bzero(&addr,sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_port=htons(port);
	if (ip == NULL)
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		addr.sin_addr.s_addr=inet_addr(ip);
	socklen_t addrlen = sizeof(addr);

	const void * ptr = buf;
	int nlen = len;
	int nsend = 0;

	while(nlen > 0)
	{	//EAGAIN or EWOULDBLOCK
		nsend=sendto(sockfd,ptr,nlen,flags,(struct sockaddr*)&addr,addrlen);
		if (nsend < 0)
		{
			if (errno == EINTR)//被中断了
				nsend = 0;//这里可以改为continue
			else if(errno == EAGAIN || errno == EWOULDBLOCK)  //如果是异步的
				break;
			else
				return -1;
		}
		else if(nsend == 0)
			break;

		nlen -=nsend;
		ptr += nsend;
	}
	return len - nlen;
}
int udpSend(int sockfd,const void *buf,const int len,int flags,const struct sockaddr * addr,socklen_t addrlen)
{

	const void * ptr = buf;
	int nlen = len;
	int nsend = 0;

	while(nlen > 0)
	{	//EAGAIN or EWOULDBLOCK
		nsend=sendto(sockfd,ptr,nlen,flags,addr,addrlen);
		if (nsend < 0)
		{
			if (errno == EINTR)//被中断了
				nsend = 0;//这里可以改为continue
			else if(errno == EAGAIN || errno == EWOULDBLOCK)  //如果是异步的
				break;
			else
				return -1;
		}
		else if(nsend == 0)
			break;

		nlen -=nsend;
		ptr += nsend;
	}
	return len - nlen;
}

int udpRecv(int sockfd, void *buf, int len,int flags)
{

	int nrecv = 0;
	

	while( len > 0 )
	{
		nrecv = recvfrom(sockfd,buf,len,flags,NULL,NULL);
		if(nrecv < 0)
		{
			if (errno == EINTR)//被中断
				continue;
			else if(errno == EAGAIN || errno == EWOULDBLOCK)  //如果是异步的
				return 0;
			else
				return -1;
		}
		else if(nrecv == 0)
			return 0;
		else
			return nrecv;
	}

}

int udpRecv(int sockfd,void *buf, int len,int flags,struct sockaddr* pt_addr,socklen_t* pt_addrlen)
{
	int nrecv = 0;
	
	while( len > 0 )
	{
		nrecv = recvfrom(sockfd,buf,len,flags,pt_addr,pt_addrlen);
		if(nrecv < 0)
		{
			if (errno == EINTR)//被中断
				continue;
			else if(errno == EAGAIN || errno == EWOULDBLOCK)  //如果是异步的
				return 0;
			else
				return -1;
		}
		else if(nrecv == 0)
			return 0;
		else
			return nrecv;
	}
}




