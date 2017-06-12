#include "stdio.h"
#include "stdlib.h"
#include "v_rbtree.h"


#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <errno.h>
#include <pthread.h>

#include <sys/epoll.h>

#define bool int
#define true 1
#define false 0

struct gateway_handle
{
	void (*open) ();
	void (*connect) (const int fd,const char * addr);
	void (*disconnect) (const int fd);
	void (*error) (const int fd,const char * msg);
	void (*message) (const int fd,const void * msg,const int size);
};

typedef struct fd_data_struct{
    v_rbtree_node_t					node;
	int								fd;
	void 							*porigin;
	void 							*ptail;
    void 							*pdata;
	int 							size;
}fd_data_struct_t;


int set_config(const char * host,const int port ,const int max_client ,const bool nodelay );
int start(const struct gateway_handle handle);



static int 			work();
static int 			socketinit(char *ip,int port);
static void			gclose(int fd);
static int 			set_socket_nonblock(int fd);

static int 					init_node(const int fd);
static fd_data_struct_t* 	find_node(const int fd);
static void 				del_node(const int fd);