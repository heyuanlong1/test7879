#ifndef __LOBBY_MYSQL_H__
#define __LOBBY_MYSQL_H__

#include "mysql.h"

int lobby_db_model_reg_user(const char *name,const char *email);
int lobby_db_model_is_reg_user(int user_id);

#endif

/*

create table `user`(
`userID` int(11) NOT NULL AUTO_INCREMENT COMMENT '自增的user_id',
`name` varchar(50) NOT NULL DEFAULT '',
`password` varchar(50) NOT NULL DEFAULT '',
`email` varchar(50) NOT NULL DEFAULT '',
`src` varchar(50) NOT NULL DEFAULT '' COMMENT '来源',
`inputTime` varchar(19) NOT NULL DEFAULT '0000-00-00 00:00:00',
primary key (`userID`),
unique key `key_unique` (`email`)
)ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;

*/