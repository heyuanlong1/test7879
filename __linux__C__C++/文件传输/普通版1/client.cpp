#include "base.h"
#include "client.h"

#define MAXLINE 1024

int main(int argc, char const *argv[])
{
	char ip[32] = {0};
	int port;
	char fileName[1024] = {0};
	char buf[MAXLINE];
	FILE *fp = NULL;
	int sockLink = -1;

	if (argc < 4){
		printf("arg is too less\n");
		return 0;
	}

	strcpy(ip,argv[1]);
	port  = atoi(argv[2]);
	strcpy(fileName,argv[3]);

	fp = openFile(fileName);
	if (fp == NULL){
		return 0;
	}
	sockLink = connectServer(ip,port);
	if (sockLink == -1 ){
		fclose(fp);
		return 0;
	}

	//这个有个bug，粘包没处理
	int sendLen = send(sockLink,fileName,strlen(fileName),0);
	if (sendLen < 0){
		perror("send file name");
		fclose(fp);
		close(sockLink);
		return 0 ;
	}

	int readLen;
	while( (readLen = fread(buf,sizeof(char),MAXLINE,fp)) > 0 ){
		sendLen = send(sockLink,buf,readLen,0);
		if (sendLen < 0){
			perror("send file name");
			fclose(fp);
			close(sockLink);
			return 0 ;
		}
	}
	
	fclose(fp);
	close(sockLink);
	return 0;
}


FILE* openFile(char *file)
{
	FILE *temp = fopen(file,"r");
	if (temp == NULL){
		perror("open file");
		return NULL;
	}
	return temp;

}
int connectServer(char *ip,int port)
{
	printf("ip:%s,port:%d\n",ip,port );
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