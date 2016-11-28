#ifndef __SERVER_H_
#define __SERVER_H_

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
#include <list>
#include <deque>
#include "mysql.h"
#include "mysqlPool.h"
#include "mutex.h"
#include "controlUnit.h"

int socketinit(int port);
int sendMsg(int originFd,const char *msg,int msgLen);
void* deal(void *param);
void* calculate(void *param);

int getNewClientArr(const char *buf,const char sep,int *arrFd,int *arrNums);
#endif