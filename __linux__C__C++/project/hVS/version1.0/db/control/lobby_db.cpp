#include "lobby_db.h"
#include "db/model/mysql/lobby_mysql.h"
int lobby_db_control_reg_user(const char *name, const char *email)
{
	return lobby_db_model_reg_user(name,email);
}
int lobby_db_control_is_reg_user(int user_id)
{
	return lobby_db_model_is_reg_user(user_id);
}