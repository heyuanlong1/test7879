#ifndef __CLIENT_H_
#define __CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <errno.h>

#include <sys/epoll.h>
#include <pthread.h>

int connectServer(char *ip,int port);
void dealUserMsg(char *stdinBuf);
void dealRecvMsg(char *buf,int recvLen);

int sendLobby(const char *buf,const int len);
int sendRoom(const char *buf,const int len);


int getBufReg(char *buf,int *bufLen);
int getBufLogin(char *buf,int *bufLen);
int getBufLogout(char *buf,int *bufLen);
int getBufRoomIn(char *buf,int *bufLen);
int getBufRoomOut(char *buf,int *bufLen);
int getBufMsg(char *buf,int *bufLen);




#endif