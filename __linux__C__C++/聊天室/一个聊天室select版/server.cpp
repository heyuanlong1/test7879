#include "base.h"
#include "server.h"


#define MAXBUF 1500


int main(int argc, char const *argv[])
{

	int port;
	char buf[MAXBUF];

	std::list<int> fdList;
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
	
	fd_set fdSet,tempFdSet;
	int max_sock = 0;

	FD_ZERO(&fdSet);
	FD_SET(serversock,&fdSet);
	max_sock = serversock;


	int eventNums = 0;
	for ( ; ; )
	{
		tempFdSet = fdSet;
		eventNums = select(max_sock +1 ,&tempFdSet,NULL,NULL,NULL);
		if (eventNums < 0){
			perror("select fail");
		}

		for (int i = 0; i <= max_sock; ++i)
		{
			if ( FD_ISSET(i,&tempFdSet) == 0){
				continue;
			}

			if (i == serversock ){
				int acceptClient = accept(serversock,NULL,NULL);
				if (acceptClient == -1){
					perror("accept client");
					continue;
				}
				printf("a new client:%d\n",acceptClient);
				FD_SET(acceptClient,&fdSet);
				addList(&fdList,acceptClient);
				max_sock = max_sock > acceptClient ? max_sock:acceptClient;		

			}else{
				int recvLen = recv(i,buf,MAXBUF-1,0);
				if(recvLen > 0){
					sendMsg(&fdList,i,buf,recvLen);

				}else if (recvLen == 0){
					close(i);
					FD_CLR(i,&fdSet);
					removeList(&fdList,i);
					max_sock = getMaxFd	(&fdList,serversock);

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

int getMaxFd(std::list<int> *li,int serverFd)
{
	int max = 0;
	max = serverFd;
	std::list<int>::iterator b,e;
	b = li->begin();
	e = li->end();
	for (; b != e; ++b){
		if (*b > max){
			max = *b;
		}
	}
	return max;
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