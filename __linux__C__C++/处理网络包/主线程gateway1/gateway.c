#include "gateway.h"


#define EPOLL_SIZE 1024

#define MSGMAXSIZE 65535
#define HEADERSIZE 2

char 					g_host[20];
int 					g_port;
int 					g_max_client;
bool 					g_nodelay;
struct gateway_handle 	g_handle;
int 					g_epoll	= -1 ;
int 					g_online = 0;


v_thread_volatile v_rbtree_t		 g_fd_data_rbtree;
static v_rbtree_node_t				 v_sentinel;

int set_config(const char * host,const int port ,const int max_client ,const bool nodelay)
{
	strcpy(g_host,host);
	g_port = port;
	g_max_client = max_client;
	g_nodelay = nodelay;
	return 0;
}
int start(const struct gateway_handle handle)
{
	g_handle.open = 		handle.open;
	g_handle.connect = 		handle.connect;
	g_handle.disconnect = 	handle.disconnect;
	g_handle.error = 		handle.error;
	g_handle.message = 		handle.message;

	v_rbtree_init(&g_fd_data_rbtree, &v_sentinel,v_rbtree_insert_value);

	return work();

}


int work()
{
	int fd = -1;
	int server_df = -1;
	int ready_event_nums = 0 ;
	int i = 0;
	int acceptClient = -1;
	int recvLen = -1;
	struct epoll_event server_epoll_event;
	struct epoll_event temp_event;
	struct epoll_event ready_event[EPOLL_SIZE];


	server_df=socketinit(g_host,g_port);
	if (server_df == -1)
	{
		g_handle.error(-1,"socketinit fail");
		return 1;
	}
	g_epoll = epoll_create(EPOLL_SIZE);
	if(g_epoll == -1){
		g_handle.error(-1,"epoll create fail");
		close(server_df);
		return 1;
	}
	server_epoll_event.events = EPOLLIN;
	server_epoll_event.data.fd = server_df;
	if (epoll_ctl(g_epoll,EPOLL_CTL_ADD,server_df,&server_epoll_event) == -1){
		g_handle.error(-1,"add epoll fail");
		close(server_df);
		return 1;
	}
	++g_online;

	for ( ; ; )
	{
		ready_event_nums = epoll_wait(g_epoll,ready_event,EPOLL_SIZE,-1);
		for ( i = 0; i < ready_event_nums; ++i)
		{
			fd = ready_event[i].data.fd;
			if (!(ready_event[i].events & EPOLLIN)){
				continue;
			}
			if (fd == server_df ){
				acceptClient = accept(server_df,NULL,NULL);
				if (acceptClient == -1){
					g_handle.error(-1,"accept client fail");
					continue;
				}

				temp_event.events = EPOLLIN;
				temp_event.data.fd = acceptClient;
				if ((g_online+1) > EPOLL_SIZE){
					g_handle.error(acceptClient,"more than EPOLL_SIZE");
					continue;
				}
				if (epoll_ctl(g_epoll,EPOLL_CTL_ADD,acceptClient,&temp_event) == -1){
					g_handle.error(acceptClient,"epoll add acceptClient fail");
					continue;
				}
				++g_online;
				init_node(acceptClient);
				g_handle.connect(acceptClient,"");

			}else{
				fd_data_struct_t* n = find_node(fd);
				recvLen = recv(fd,(n->pdata + n->size),(n->ptail - n->pdata - n->size),0);
				
				if(recvLen > 0){
					n->size += recvLen;
					if((n->pdata + n->size) == n->ptail && n->pdata != n->porigin){   //
						memcpy(n->porigin,n->pdata,n->size);
						n->pdata = n->porigin;
					}
					if(n->size >= HEADERSIZE){
						unsigned short *s_packet_size = (unsigned short *)n->pdata;
						int i_packet_size = (int)(*s_packet_size);
						if(i_packet_size > MSGMAXSIZE  ){
							gclose(fd);
							g_handle.error(fd,"packet size too big");
						}
						if(n->size >= i_packet_size ){
							g_handle.message(fd,n->pdata,i_packet_size);
							n->pdata += i_packet_size;
							n->size -= i_packet_size;
						}
					}
				}else if (recvLen == 0){
					gclose(fd);
					g_handle.disconnect(fd);
				}else{
					gclose(fd);
					g_handle.error(fd,"client fail");
				}
			}
		}
	}
	return 0;
}

void gclose(int fd)
{
	epoll_ctl(g_epoll,EPOLL_CTL_DEL,fd,NULL);
	del_node(fd);
	close(fd);
	--g_online;
}


//------------------------------rbtree----------------------------------
int init_node(const int fd)
{
	fd_data_struct_t		*n = (fd_data_struct_t*)malloc(sizeof(fd_data_struct_t));
	v_rbtree_node_t		*node;

    n->fd = fd;
    n->porigin = (void*) malloc(MSGMAXSIZE );
	n->ptail = n->porigin + (MSGMAXSIZE );
	n->pdata = n->porigin;
	n->size = 0;
	node = &n->node;
	node->key.v = fd;
	node->key.v2 = 0;
	v_rbtree_insert(&g_fd_data_rbtree, node);

	return 0;
}

fd_data_struct_t* find_node(const int fd)
{
	v_rbtree_node_t						*node;
	fd_data_struct_t					*n;
    v_rbtree_key_t                      key;

 	key.v = fd;
    key.v2 = 0;

    if (v_OK != v_rbtree_get(&g_fd_data_rbtree, key, &v_sentinel, &node)) {
        return NULL;
    }
    n = container_of(node, fd_data_struct_t, node);
	return n;
}


void del_node(const int fd)
{
	fd_data_struct_t	*n;

	n = find_node(fd);
	if (n != NULL)
	{
		v_rbtree_delete(&g_fd_data_rbtree, &n->node);
		free(n->porigin);
		free(n);
	}
}

//------------------------------socket---------------------------------

int socketinit(char *ip,int port)
{
	int tempSock = socket(AF_INET,SOCK_STREAM,0);
	if(tempSock == -1){
		//perror("init socket");
		return -1;
	}
	struct sockaddr_in serverAddr;
	bzero(&serverAddr,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr  =  inet_addr(ip);

	int val=1;
    if (0 != setsockopt(tempSock,SOL_SOCKET,SO_REUSEADDR,(const void*)&val,sizeof(int))){
		//perror("setsockopt");
		return -1;
    }

	if(bind(tempSock,(struct sockaddr *)&serverAddr,sizeof(struct sockaddr)) == -1){
		//perror("bind");
		return -1;
	}
	if (listen(tempSock,128) == -1)
	{
		//perror("listen");
		return -1;
	}
	return tempSock;

}

int set_socket_nonblock(int fd)
{
   return fcntl(fd,F_SETFL,fcntl(fd,F_GETFL,0) | O_NONBLOCK);
}


//---------------------------------------------------------------