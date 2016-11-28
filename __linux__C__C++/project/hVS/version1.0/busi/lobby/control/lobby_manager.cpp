#include "lobby_manager.h"
#include "event/event.h"
#include "log/log.h"
lobby_t *s_lobby;


int busi_lobby_callback_handle(lobby_busi_t *busi);
int busi_lobby_reg(lobby_busi_t* busi);
int busi_lobby_login(lobby_busi_t* busi);
int busi_lobby_logout(lobby_busi_t* busi);
int busi_lobby_enter_room(lobby_busi_t* busi);
int busi_lobby_goout_room(lobby_busi_t* busi);



int busi_lobby_init()
{

	s_lobby = (lobby_t*)malloc(sizeof(lobby_t));
	s_lobby->lobby_id = 1;
	s_lobby->room_nums = 0;

}

int busi_lobby_loop()
{
	event_lobby_init();
	int res = event_lobby_loop(busi_lobby_callback_handle); //里面是循环
	return res;
}



int busi_lobby_callback_handle(lobby_busi_t *busi)
{
	static int  head_size = sizeof(lobby_busi_head_t);

	if (busi->data_len < head_size) {
		printf("net_upd_recv recv_len < head_size:%d < %d\n", busi->data_len, head_size);
		return 0;
	}

	lobby_busi_head_t *head = (lobby_busi_head_t*)busi->data;
	if (busi->data_len < head->size) {
		printf("net_upd_recv recv_len < head->size:%d < %d\n", busi->data_len, head->size);
		return 0;
	}
	
	log_info_fd("busi_lobby_callback_handle from_type:%d,cmd:%d\n", head->from_type, head->cmd);
	switch (head->from_type)
	{
	case FROM_TYPE_CLIENT:
		switch (head->cmd)
		{
		case CMD_REG:
			busi_lobby_reg(busi);
			break;
		case CMD_LOGIN:
			busi_lobby_login(busi);
			break;
		case CMD_LOGOUT:
			busi_lobby_logout(busi);
			break;
		case CMD_ENTER_ROOM:
			busi_lobby_enter_room(busi);
			break;
		default:
			break;
		}
		break;
	case FROM_TYPE_ROOM_SERVER:
		switch (head->cmd)
		{
		case CMD_GOOUT_ROOM:
			busi_lobby_goout_room(busi);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
int busi_lobby_reg(lobby_busi_t* busi)
{
	lobby_busi_reg_t *reg = (lobby_busi_reg_t*)busi->data;
	char name[51] = {0};
	char email[51] = { 0 };

	int user_id;
	lobby_callback_t cback;
	cback.m_head.size = sizeof(lobby_callback_t);
	cback.m_head.from_type = TO_TYPE_CLIENT;
	cback.m_head.cmd = BACK_REG;
	cback.user_id = user_id;
	cback.room_id = 0;

	sprintf(name, "%s", reg->name);
	sprintf(email, "%s", reg->email);

	user_id = busi_lobby_model_reg(name, email);
	if (user_id < 0) {
		cback.status = HVA_ERROR;
		cback.code = SOME_ERROR;	
	}
	else {
		cback.status = HVS_OK;
	}

	busi->data = (void*)(&cback);
	busi->data_len = cback.m_head.size;
	event_lobby_notify(busi);

	return 0;
}
int busi_lobby_login(lobby_busi_t* busi)
{
	lobby_login_t *login = (lobby_login_t*)busi->data;
	int res;
	int is_reg;
	lobby_callback_t cback;
	cback.m_head.size = sizeof(lobby_callback_t);
	cback.m_head.from_type = TO_TYPE_CLIENT;
	cback.m_head.cmd = BACK_LOGIN;
	cback.user_id = login->user_id;
	cback.room_id = 0;
	busi->data = (void*)(&cback);
	busi->data_len = cback.m_head.size;

	is_reg = busi_lobby_model_is_reg(login->user_id);
	if (is_reg < 0) {
		cback.status = HVA_ERROR;
		cback.code = THIS_USER_ID_IS_NOT_REG;
	}
	else {
		res = busi_lobby_model_login(s_lobby, login->user_id);
		if (res < 0) {
			cback.status = HVA_ERROR;
			cback.code = HAD_LOGIN_LOBBY;
		}
		else {
			cback.status = HVS_OK;
		}
	}

	event_lobby_notify(busi);
	return 0;
}
int busi_lobby_logout(lobby_busi_t* busi)
{
	lobby_logout_t *logout = (lobby_logout_t*)busi->data;
	int res;
	int is_login;
	lobby_callback_t cback;
	cback.m_head.size = sizeof(lobby_callback_t);
	cback.m_head.from_type = TO_TYPE_CLIENT;
	cback.m_head.cmd = BACK_LOGIN;
	cback.user_id = logout->user_id;
	cback.room_id = logout->room_id;
	busi->data = (void*)(&cback);
	busi->data_len = cback.m_head.size;
	is_login = busi_lobby_model_is_login(s_lobby, logout->user_id);
	if (is_login < 0) {
		cback.status = HVA_ERROR;
		cback.code = NOT_LOGIN_LOBBY;
	}
	else {
		res = busi_lobby_model_logout(s_lobby, logout->user_id);
		if (res < 0) {
			cback.status = HVA_ERROR;
			cback.code = NOT_LOGIN_LOBBY;
		}
		else {
			cback.status = HVS_OK;
		}
	}
	
	event_lobby_notify(busi);
	return 0;
}
int busi_lobby_enter_room(lobby_busi_t* busi)
{
	int room_id;
	int is_login;
	is_login = busi_lobby_model_is_login(s_lobby,11);
	if (is_login < 0) {

	}
	room_id = busi_lobby_model_enter_room(11,0);
	if (room_id < 0) {

	}
	//回复用户等待。
	return 0;
}
int busi_lobby_goout_room(lobby_busi_t* busi)
{
	int res;
	int is_login;
	is_login = busi_lobby_model_is_login(s_lobby,11);
	if (is_login < 0) {

	}
	res = busi_lobby_model_goout_room(11,0);
	if (res < 0) {

	}
	//回复用户等待。
	return 0;
}