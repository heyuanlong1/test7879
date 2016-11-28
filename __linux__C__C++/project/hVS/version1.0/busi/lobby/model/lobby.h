#ifndef __LOBBY__H__
#define __LOBBY__H__

#include <iostream>
#include <map>
#include <set>
#include "busi/room/model/room.h"
#include "busi/lobby/control/lobby_manager.h"
typedef std::map<int, room_t*> room_map_t;
typedef std::set<int> user_set_t;

typedef struct lobby_s lobby_t;
struct lobby_s {
	int lobby_id;
	room_map_t room_map;
	user_set_t user_set;
	int room_nums;
};


int busi_lobby_model_reg(const char *name, const char *email);
int busi_lobby_model_is_reg(int user_id);
int busi_lobby_model_login(lobby_t *plobby,int user_id);
int busi_lobby_model_is_login(lobby_t *plobby, int user_id);
int busi_lobby_model_logout(lobby_t*plobby, int user_id);
int busi_lobby_model_enter_room(int user_id,int room_id);
int busi_lobby_model_goout_room(int user_id,int room_id);



#endif
