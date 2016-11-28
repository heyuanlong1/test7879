#ifndef __NET__H__
#define __NET__H__

#include <stdio.h>
#include <stdlib.h>

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <fcntl.h>
int net_udp_socket_init(int port);
int net_upd_recv(int socket,char *buf, int size, struct sockaddr_in* addr);
int net_upd_send(int socket, const char  *buf, const int size, struct sockaddr_in* addr);

#endif 