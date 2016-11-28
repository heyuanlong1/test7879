#ifndef __LOBBY__DB_H__
#define __LOBBY__DB_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lobby_db_control_reg_user(const char *name, const char *email);
int lobby_db_control_is_reg_user(int user_id);




#endif