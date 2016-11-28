#include "event.h"
#define LOBBY_NET_UPD_BUF_SIZE 1500

static int lobby_server_upd_socket = -1;

int event_lobby_init()
{
	int port = get_lobby_server_udp_port();
	lobby_server_upd_socket = net_udp_socket_init(port);
	printf("event_lobby_init lobby_server_upd_socket:%d\n", lobby_server_upd_socket);
	return 0;
}
int event_lobby_loop(busi_lobby_callback_handle_t lobby_callback)
{
	
	static char net_buf[LOBBY_NET_UPD_BUF_SIZE] = { 0 };
	static int recv_len;
	static struct sockaddr_in addr;
	static lobby_busi_t busi;

	for (;;) {
		recv_len = net_upd_recv(lobby_server_upd_socket, net_buf, LOBBY_NET_UPD_BUF_SIZE, &addr);
		if (recv_len > 0) {
			busi.data = ((void*)net_buf);
			busi.data_len = recv_len;
			busi.addr = &addr;
			lobby_callback(&busi);
		}
		else {
		
		}
	}

}

int event_lobby_notify(lobby_busi_t *busi)
{
	int sendLen = net_upd_send(lobby_server_upd_socket, (const char *)busi->data, busi->data_len ,busi->addr);
	if (sendLen <= 0) {
		printf("net_upd_send failed\n");
	}
	return 0;
}



/*
int event_lobby_cmd(int cmd)
{
	switch (cmd)
	{
	case CMD_REG:
	case CMD_LOGIN:
	case CMD_LOGOUT:
	case CMD_ENTER_ROOM:
	case CMD_GOOUT_ROOM:
		break;
	default:
		return -1;
	}
	return 0;
}
*/