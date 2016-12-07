#ifndef __MS_CONFIGURE_H__
#define __MS_CONFIGURE_H__

#include <string.h>



int load();
void strTrim(char *str);
int find_first_not_of(char *str,int str_len,const char *sub,int sub_len);
int find_last_not_of(char *str,int str_len,const char *sub,int sub_len);


#endif