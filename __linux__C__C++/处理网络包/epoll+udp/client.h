#ifndef __CLIENT__
#define __CLIENT__

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





//发送完数据就返回
int setNotbolck(int sockfd);
int udpSend(int sockfd,const char *ip,const int port,const void *buf,const int len,int flags);
int udpSend(int sockfd,const void *buf,const int len,int flags,const struct sockaddr * addr,socklen_t addrlen);

//收到一个包的数据就应该返回
//服务端的recv的socket要先bind。
//而客户端的是不用的。
int udpRecv(int sockfd, void *buf, int len,int flags);
int udpRecv(int sockfd, void *buf, int len,int flags,struct sockaddr* pt_addr,socklen_t* pt_addrlen);







#endif