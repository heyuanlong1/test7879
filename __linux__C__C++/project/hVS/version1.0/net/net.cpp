#include "net.h"

int net_udp_socket_init(int port)
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("net_udp_socket_init socket failed");
		exit(1);
	}
	struct sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port);
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int opts = fcntl(sock, F_GETFL);
	if (opts < 0) {
		perror("fcntl(F_GETFL)");
		exit(1);
	}
	opts = opts & (~O_NONBLOCK);
	if (fcntl(sock, F_SETFL, opts) < 0) {
		perror("fcntl(F_SETFL)");
		exit(1);
	}

	if (bind(sock, (struct sockaddr*)&sockaddr, sizeof(struct sockaddr)) < 0) {
		perror("net_udp_socket_init bind failed");
		exit(1);
	}
	return sock;
}





int net_upd_recv(int socket, char *buf, int size, struct sockaddr_in* addr)
{
	int resLen;
	int addrLen = sizeof(struct sockaddr_in);
	resLen = recvfrom(socket, buf, size, 0,(struct sockaddr*)addr,(socklen_t*)&addrLen);
	if(resLen < 0){
		printf("socket:%d\n", socket);
		perror("recvfrom failed");
		exit(1);
	}
	return resLen;
}

int net_upd_send(int socket, const char  *buf, const int size, struct sockaddr_in* addr)
{

	int sendLen;
	sendLen = sendto(socket, buf, size, 0, (struct sockaddr*)addr, sizeof(struct sockaddr));
	return sendLen;
}