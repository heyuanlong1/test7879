#ifndef __LOBBY_MANAGER__H__
#define __LOBBY_MANAGER__H__
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include "busi/lobby/model/lobby.h"

#define FROM_TYPE_CLIENT						1000
#define CMD_REG									1001
#define CMD_LOGIN								1002
#define CMD_LOGOUT								1003
#define CMD_ENTER_ROOM							1004

#define FROM_TYPE_ROOM_SERVER					2000
#define CMD_GOOUT_ROOM							2001

#define TO_TYPE_CLIENT							1100
#define BACK_REG								1101
#define BACK_LOGIN								1102
#define BACK_LOGOUT								1103
#define BACK_ENTER_ROOM							1104

#define HVS_OK									0
#define HVA_ERROR								-1
#define SOME_ERROR								-100000
#define THIS_USER_ID_IS_NOT_REG					-100001
#define HAD_LOGIN_LOBBY							-100002
#define NOT_LOGIN_LOBBY							-100003

typedef struct ms_str_s
{
	int str_len;
	char *str;
}ms_str_t;

typedef struct lobby_busi_head_s lobby_busi_head_t;
struct lobby_busi_head_s {
	int size;
	int from_type;
	int cmd;
};

typedef struct lobby_busi_reg_s lobby_busi_reg_t;
struct lobby_busi_reg_s {
	lobby_busi_head_s m_head;
	char name[50];
	char email[50];
};

typedef struct lobby_login_s lobby_login_t;
struct lobby_login_s {
	lobby_busi_head_t m_head;
	int user_id;
	int room_id;
};
typedef lobby_login_t lobby_logout_t;






typedef struct lobby_event_s lobby_event_t;
struct lobby_event_s {
	lobby_busi_head_t m_head;
	int user_id;
	int room_id;
};

typedef struct lobby_callback_s lobby_callback_t;
struct lobby_callback_s {
	lobby_busi_head_t m_head;
	int user_id;
	int room_id;
	int status;
	int code;
};


typedef struct lobby_busi_s lobby_busi_t;
struct lobby_busi_s {
	void *data;
	int data_len;
	struct sockaddr_in *addr;
};

typedef int(*busi_lobby_callback_handle_t)(lobby_busi_t *busi);


int busi_lobby_init();
int busi_lobby_loop();

#endif