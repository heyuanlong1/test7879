#include "base.h"
#include "client.h"
#include <time.h>

#define MAXBUF 1500
char ip[32] = {0};
int port;


int main(int argc, char const *argv[])
{

	char buf[MAXBUF];
	int sockLink = -1;

	if (argc < 4){
		printf("arg is too less\n");
		return 0;
	}

	strcpy(ip,argv[1]);
	port  = atoi(argv[2]);
	int pthreadCount = atoi(argv[3]);

	pthread_attr_t attr;
	if (pthread_attr_init(&attr)){
		perror("pthread_attr_init");
		return 0;
	}
	if (pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED)){
		perror("pthread_attr_setdetachstate");
		return 0;
	}

	for ( int i= 0; i < pthreadCount ; ++i){
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
	int bufLen=0;
	int type=0;

	int sockLink = connectServer(ip,port);
	if (sockLink == -1 ){
		return 0;
	}
	int res = 0;
	int tempSendLen = 0;
	int sendLen = 0;
	for(;;){
		res = getBuf(buf,&bufLen,&type);
		tempSendLen = 0;
		for(;;){
			sendLen = send(sockLink,buf + tempSendLen ,bufLen - tempSendLen,0);
			if(sendLen > 0){
				tempSendLen += sendLen;
				if(tempSendLen == bufLen){
					break;
				}
			}else if (sendLen == 0){		
				perror("client is close");
				return 0;
			}else{
				if( errno == EAGAIN || errno == EWOULDBLOCK){
					continue;
				}else{
					close(sockLink);
					perror("client have error");
					return 0;
				}
			}
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

int getBuf(char *buf,int *bufLen,int *type)
{
	*type = (int)(random() % 4) + LOGIN;
	switch(*type){
		case LOGIN:	getBufLogin(buf,bufLen);
			break;
		case READY:	getBufReady(buf,bufLen);
			break;
		case MSG:	getBufMsg(buf,bufLen);
			break;
		case LEAVE:	getBufLeave(buf,bufLen);
			break;
		default:
			break;

	}
	return *bufLen;
}

int getBufLogin(char *buf,int *bufLen)
{
	login_t *req = (login_t*)malloc(sizeof(login_t));
	memset(req,0,sizeof(login_t));
	req->head.size = sizeof(login_t);
	req->head.cmd = LOGIN;
	req->userid = random() % 100 + 1000 ;
	strcpy(req->token,"this is token");
	memcpy(buf,(char *)req,req->head.size);
	*bufLen = req->head.size;
	free(req);
}

int getBufReady(char *buf,int *bufLen)
{
	ready_t *req = (ready_t*)malloc(sizeof(ready_t));
	memset(req,0,sizeof(ready_t));
	req->head.size = sizeof(ready_t);
	req->head.cmd = READY;
	req->userid = random() % 100 + 1000 ;
	memcpy(buf,(char *)req,req->head.size);
	*bufLen = req->head.size;
	free(req);
}

int getBufMsg(char *buf,int *bufLen)
{
	const char * data="this is msg!!!";
	int msgLen = strlen(data);
	int packetLen = sizeof(msg_t) + msgLen;

	msg_t *req = (msg_t*)malloc(packetLen);
	memset(req,0,sizeof(msg_t));
	req->head.size = packetLen;
	req->head.cmd = MSG;
	req->userid = random() % 100 + 1000 ;	
	req->dataSize = msgLen;
	memcpy(req->data,data,msgLen);
	memcpy(buf,(char *)req,req->head.size);
	*bufLen = req->head.size;
	free(req);
}

int getBufLeave(char *buf,int *bufLen)
{
	leave_t *req = (leave_t*)malloc(sizeof(leave_t));
	memset(req,0,sizeof(leave_t));
	req->head.size = sizeof(leave_t);
	req->head.cmd = LEAVE;
	req->userid = random() % 100 + 1000 ;
	req->score = random() % 100 + 1000 ;
	memcpy(buf,(char *)req,req->head.size);
	*bufLen = req->head.size;
	free(req);
}
