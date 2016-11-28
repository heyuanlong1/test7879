#ifndef __BASE__
#define __BASE__


#define FROM_TYPE_CLIENT 1000
#define CMD_REG 1001
#define CMD_LOGIN 1002
#define CMD_LOGOUT 1003
#define CMD_ENTER_ROOM 1004

#define FROM_TYPE_ROOM_SERVER 2000
#define CMD_GOOUT_ROOM 2001

#define TO_TYPE_CLIENT 1100
#define BACK_REG 1101
#define BACK_LOGIN 1102
#define BACK_LOGOUT 1103
#define BACK_ENTER_ROOM 1104

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
};


typedef struct lobby_busi_msg_s lobby_busi_msg_t;
struct lobby_busi_msg_s {
	lobby_busi_head_s m_head;
	int data_size;
	char data[0];
};


#endif