#ifndef __CONF__H__
#define __CONF__H__

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int conf_init();
int get_room_server(char *server_arr, int *nums);
int get_lobby_server_udp_port();

int get_error_fd();
int get_warning_fd();
int get_info_fd();

const char *get_mysql_host();
const char *get_mysql_user();
const char *get_mysql_password();
const char *get_mysql_db();


#endif 