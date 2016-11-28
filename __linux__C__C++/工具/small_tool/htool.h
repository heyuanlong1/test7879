#ifndef H_TOOL__H_
#define H_TOOL__H_



#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h>
#include <sys/socket.h>
#include <stdarg.h>


void msSleep(int millSec);

long getFileSize(const char *path);
int checkIsDir(const char *path);
int checkIsFile(const char *path);


int getIPStrByHost(const char * hostName,char *IPStr,int len);

void logInfo(char *fmt,...);
void logError(char *fmt,...);
#endif