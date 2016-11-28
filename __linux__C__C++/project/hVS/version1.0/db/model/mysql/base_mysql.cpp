#include "base_mysql.h"
#include "conf/conf.h"
#include "log/log.h"
static MYSQL *mysql_connect = NULL;

static int mysql_init();

MYSQL *get_mysql_connect()
{
	if (NULL == mysql_connect) {
		if (mysql_init() < 0) {
			log_error_fd("mysql_init");
			return NULL;
		}
	}
	if (mysql_ping(mysql_connect) != 0) {
		log_warning_fd("mysql_ping failed\n");
		if (mysql_real_connect(mysql_connect, get_mysql_host(), get_mysql_user(), get_mysql_password(), get_mysql_db(), 0, NULL, 0) == NULL) {
			log_error_fd("mysql reconnect failed,mysql_errno:%d, mysql_error : %s\n", mysql_errno(mysql_connect), mysql_error(mysql_connect));
			return NULL;
		}
	}
	return mysql_connect;
}

int mysql_init() 
{
	mysql_connect = (MYSQL*)malloc(sizeof(MYSQL));
	if (mysql_init(mysql_connect) == NULL) {
		log_error_fd("mysql_init failed\n");
		return -1;
	}
	if(mysql_real_connect(mysql_connect, get_mysql_host(), get_mysql_user(), get_mysql_password(), get_mysql_db(), 0, NULL, 0) == NULL){
		log_error_fd("mysql_real_connect failed,mysql_errno:%d, mysql_error : %s\n", mysql_errno(mysql_connect), mysql_error(mysql_connect));
		return -1;
	}

	if (mysql_set_character_set(mysql_connect, "utf8") != 0) {
		log_warning_fd("mysql_set_character_set\n");
	}
	return 0;
}