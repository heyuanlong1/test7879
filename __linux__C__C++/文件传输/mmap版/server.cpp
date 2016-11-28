#include "base.h"
#include "server.h"


#define MAXLINE 1024

int main(int argc, char const *argv[])
{

	int port;
	char fileName[1024] = {0};
	char buf[MAXLINE];
	int fp = -1;
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
		if (fp == -1){
			perror("createFile");
			continue;
		}
		unsigned long fileSize = 0;
		recv(clientLink,&fileSize,sizeof(fileSize),0);
		if (fileSize == 0){
			perror("get fileSize");
			close(fp);
			continue;
		}


		if (lseek(fp,fileSize-1,SEEK_SET) < 0){
			perror("lseek");
			close(fp);
			continue;
		}
		if(write(fp,"\0",1) < 0 ){
			perror("write 0 failed");
			close(fp);
			continue;
		}
		char *mmapFile = (char *)mmap(NULL,fileSize,PROT_READ | PROT_WRITE ,MAP_SHARED,fp,0);
		if (mmapFile == MAP_FAILED){
			perror("mmap");
			close(fp);
			continue;
		}

		unsigned long recvFileSize = 0;
		while((recvLen = recv(clientLink,mmapFile + recvFileSize ,fileSize - recvFileSize,0)) > 0){
			recvFileSize += recvLen;
		}
		if (recvFileSize != fileSize){
			perror("recv file !");
			munmap(mmapFile,fileSize);
			close(fp);
			continue;
		}
		munmap(mmapFile,fileSize);
		close(fp);
	}

	close(serversock);
	return 0;
}

int createFile(char *fileName)
{
	int temp = open(fileName,O_RDWR | O_CREAT,0664);
	if (temp < 0 ){
		perror("open");
		return -1;
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