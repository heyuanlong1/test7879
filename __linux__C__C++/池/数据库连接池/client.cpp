#include "base.h"
#include "client.h"
#include <time.h>

#define MAXBUF 150
char ip[32] = {0};
int port;

void * deal(void *param);
int main(int argc, char const *argv[])
{

	char buf[MAXBUF];
	int sockLink = -1;

	if (argc < 3){
		printf("arg is too less\n");
		return 0;
	}

	strcpy(ip,argv[1]);
	port  = atoi(argv[2]);

	pthread_attr_t attr;
	if (pthread_attr_init(&attr)){
		perror("pthread_attr_init");
		return 0;
	}
	if (pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED)){
		perror("pthread_attr_setdetachstate");
		return 0;
	}

	for ( int i= 0; i < 1000 ; ++i){
		pthread_t threadId;
		int preturn;
		preturn = pthread_create(&threadId,&attr,deal,NULL);
		if (preturn != 0){
			perror("pthread_create");
			continue;
		}
	}
	read(STDOUT_FILENO,buf,MAXBUF);

	return 0;
}


void * deal(void *param)
{
	char buf[MAXBUF];
	int sockLink = connectServer(ip,port);
	if (sockLink == -1 ){
		return 0;
	}
	for(;;){
		int recvLen = recv(sockLink,buf,MAXBUF-1,0);
		if(recvLen > 0){
			write(STDOUT_FILENO,buf,recvLen);
		}else if (recvLen == 0){
			
			printf("client is close");
			break;
		}else{
			perror("client have error");
			return 0;
		}
	}
	close(sockLink);
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