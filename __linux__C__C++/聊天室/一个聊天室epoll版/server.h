#ifndef __SERVER__
#define __SERVER__

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
#include <list>

int socketinit(int port);
int sendMsg(std::list<int> *li,int originFd,char *msg,int msgLen);

int addList(std::list<int> *li,int fd);
int removeList(std::list<int> *li,int fd);
#endif