#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <unistd.h>         //access
#include <stdarg.h>         //vsnprintf,va_start,va_end
#include <sys/stat.h>       //mkdir
#include <sys/time.h>       //gettimeofday

enum {
    KO_LOG_LEVEL_ERROR     = 1,  //错误
    KO_LOG_LEVEL_WARNING   = 2,  //警告
    KO_LOG_LEVEL_INFO      = 3,  //普通
    KO_LOG_LEVEL_DEBUG     = 4,  //调试
    KO_LOG_LEVEL_MAX
};

#define KO_LOG_SIZE_LIMIT       104857600			//100 * 1024 * 1024static int ko_log_error_fd = 0;

static int ko_log_sys_fd = 0;
static int ko_log_app_fd = 0;

static int ko_log_sys_size = 0;
static int ko_log_app_size = 0;

static int ko_log_level = 0;

static char  path_str[ 256 ];
static int   path_len = 0;

char* get_time_str(bool is_write);
int ko_log_real( int wfd, int pos, int level, const char* format, va_list arg_list );
int ko_log_turn_sys();
int ko_log_turn_app();


int ko_log_init()
{
	int						sys_fd;
	int						app_fd;
	
	char					buffer[256];
	int 					len = 0;
	const char* 			path = KO_LOG_PATH;
	char* 					pos;
	
	stpcpy(buffer,path);
	len =  strlen(buffer);
	pos = buffer + len;
	
	strcpy(pos,"/ko_sys_log");
	sys_fd = open(buffer,O_WRONLY |O_CREAT|O_APPEND,0644);
	if( sys_fd <= 0 ){
		printf( "error log :%s could not be opened\n", buffer );
		return -1;
	}
	
	strcpy(pos,"/ko_app_log");
	app_fd = open(buffer,O_WRONLY |O_CREAT|O_APPEND,0644);
	if( app_fd <= 0 ){
		printf( "error log :%s could not be opened\n", buffer );
		return -1;
	}
	
	
	ko_log_sys_fd = sys_fd;
	ko_log_app_fd = sys_fd;
	
	return 0;
}

int ko_log_turn_sys()
{
	char*							buffer;
	char*							pos;
	int								sys_fd;
	
	buffer = path_str;
	sys_fd = 0;
	pos = buffer + path_len;
	
	sprintf(pos, "/ko_sys_log.%s", get_time_str(false));
	sys_fd = open(buffer,O_WRONLY|O_CREAT|O_TRUNC,0644);
	if( sys_fd <= 0 ){
		printf( "error log :%s could not be opened\n", buffer );
		return -1;
	}
	
	if (ko_log_sys_fd){
		close(ko_log_sys_fd);
	}

	ko_log_sys_fd = sys_fd;
	ko_log_sys_size = 0;
	
	return 0;
	
}
int ko_log_turn_app()
{
	char*							buffer;
	char*							pos;
	int								app_fd;
	
	buffer = path_str;
	app_fd = 0;
	pos = buffer + path_len;
	
	sprintf(pos, "/ko_app_log.%s", get_time_str(false));
	app_fd = open(buffer,O_WRONLY|O_CREAT|O_TRUNC,0644);
	if( app_fd <= 0 ){
		printf( "error log :%s could not be opened\n", buffer );
		return -1;
	}
	
	if (ko_log_app_fd){
		close(ko_log_app_fd);
	}

	ko_log_app_fd = sys_fd;
	ko_log_app_size = 0;
	
	return 0;
}

int ko_log_set_path( const char* path )
{
	stpcpy(path_str,path);
	path_len = strlen(path_str);
	
	if( ko_log_turn_sys() ){
		return -1;
	}
	if( ko_log_turn_app() ){
		return -1;
	}
	return 0;
}

int ko_log_set_path_for_printf()
{
	ko_log_sys_fd = 0;
	ko_log_sys_size = 0;
	ko_log_app_fd = 0;
	ko_log_app_size = 0;
}

int ko_log_close()
{
	if( ko_log_sys_fd ){
		close( ko_log_sys_fd );
		ko_log_sys_fd = 0;
	}

	if( ko_log_app_fd ){
		close( ko_log_app_fd );
		ko_log_app_fd = 0;
	}

	return 0;
}




char* get_time_str(bool is_write)
{
    time_t now = {0};
    struct tm *ptime = NULL;
    time(&now);
    ptime = localtime(&now);
    memset(m_time_str, 0, sizeof(m_time_str));

    uint32 milisec = 0;
#ifdef WIN32
    SYSTEMTIME wtm;
    GetLocalTime(&wtm);
    milisec = wtm.wMilliseconds;
#else
    struct timeval tv = {0};
    gettimeofday(&tv, 0);
    milisec = tv.tv_usec/1000;
#endif

    if (is_write)
    {//用来写日志
        sprintf(m_time_str, "%04d%02d%02d-%02d:%02d:%02d.%06ld",
            (1900+ptime->tm_year), (1+ptime->tm_mon), ptime->tm_mday,
            ptime->tm_hour, ptime->tm_min, ptime->tm_sec, milisec);
    }
    else
    {//用来重命名文件
        sprintf(m_time_str, "%04d-%02d-%02d-%02d_%02d_%02d_%06ld",
            (1900+ptime->tm_year), (1+ptime->tm_mon), ptime->tm_mday,
            ptime->tm_hour, ptime->tm_min, ptime->tm_sec, milisec);
    }

    return m_time_str;
}


















//