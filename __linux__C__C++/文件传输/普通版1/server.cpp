#include "base.h"
#include "server.h"


#define MAXLINE 1024

int main(int argc, char const *argv[])
{

	int port;
	char fileName[1024] = {0};
	char buf[MAXLINE];
	FILE *fp = NULL;
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

	for ( ; ; )
	{
		int clientLink = accept(serversock,NULL,NULL);
		if (clientLink == -1){
			perror("accpet");
			close(serversock);
			return 0;
		}
		int recvLen;
		recvLen = recv(clientLink,fileName,sizeof(fileName),0);
		if (recvLen <= 0 )
		{
			perror("recv fileName");
			continue;
		}
		fp = createFile(fileName);
		if (fp == NULL){
			perror("createFile");
			continue;
		}
		int writeLen;
		while(recvLen = recv(clientLink,buf,MAXLINE,0)){
			if (recvLen <= 0 ){
				perror("recv fileName");
				fclose(fp);
				continue;
			}
			writeLen = fwrite(buf,sizeof(char),recvLen,fp);
			if (writeLen < recvLen){
				perror("write file failed");
				fclose(fp);
				continue;
			}
		}
		fclose(fp);
	}

	close(serversock);
	return 0;
}

FILE * createFile(char *fileName)
{
	FILE *temp = fopen(fileName,"w+");
	if (temp == NULL){
		perror("fopen");
		return NULL;
	}
	return temp;
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