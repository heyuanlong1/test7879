#include "base.h"
#include "client.h"
#include <time.h>

#define MAXBUF 1500


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

	fd_set fdSet,tempSet;
	FD_SET(sockLink,&fdSet);
	FD_SET(STDIN_FILENO,&fdSet);
	int maxSock = sockLink > STDIN_FILENO ? sockLink:STDIN_FILENO;

	int EventNums = 0 ;
	for ( ; ; )
	{
		tempSet =fdSet;
		EventNums = select(maxSock + 1 ,&tempSet,NULL,NULL,NULL);
		for (int i = 0; i <= maxSock; ++i)
		{
			if ( FD_ISSET(i,&tempSet) == 0){
				continue;
			}

			if (i == sockLink ){
				int recvLen = recv(i,buf,MAXBUF-1,0);
				if(recvLen > 0){
					write(STDOUT_FILENO,buf,recvLen);

				}else if (recvLen == 0){
					close(i);
					FD_CLR(i,&fdSet);
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
					close(i);
					FD_CLR(i,&fdSet);
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