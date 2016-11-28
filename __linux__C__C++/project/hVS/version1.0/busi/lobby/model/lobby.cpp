#include "lobby.h"
#include "log/log.h"
#include "db/control/lobby_db.h"

static int add_user_to_lobby(int user_id);


int busi_lobby_model_reg(const char *name, const char *email)
{
	int user_id = lobby_db_control_reg_user(name,email);
	if (user_id < 0) {
		printf("busi_lobby_model_reg failed\n");
		return -1;
	}
	return user_id;
}
int busi_lobby_model_is_reg(int user_id)
{
	return lobby_db_control_is_reg_user(user_id);
}
int busi_lobby_model_login(lobby_t *plobby,int user_id)
{
	user_set_t::iterator it;
	it = plobby->user_set.find(user_id);
	if (it != plobby->user_set.end()) {
		return HAD_LOGIN_LOBBY;
	}
	plobby->user_set.insert(user_id);
	return 0;
}
int busi_lobby_model_is_login(lobby_t*plobby,int user_id)
{
	user_set_t::iterator it;
	it = plobby->user_set.find(user_id);
	if (it != plobby->user_set.end()) {
		return 0;
	}
	return NOT_LOGIN_LOBBY;
}
int busi_lobby_model_logout(lobby_t*plobby, int user_id)
{
	return 0;
}
int busi_lobby_model_enter_room(int user_id, int room_id)
{
}
int busi_lobby_model_goout_room(int user_id, int room_id)
{
}

