#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <unistd.h>         //access
#include <stdarg.h>         //vsnprintf,va_start,va_end
#include <sys/stat.h>       //mkdir
#include <sys/time.h>       //gettimeofday
#include <fcntl.h>


static int ko_log_sys_fd = 0;
static int ko_log_app_fd = 0;

static int ko_log_sys_size = 0;
static int ko_log_app_size = 0;

static int ko_log_level = KO_LOG_LEVEL_MAX;

static char  m_time_str[MAX_TIME_STR_LEN];
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
	
	strcpy(buffer,path);
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
	ko_log_app_fd = app_fd;
	
	return 0;
}

int ko_log_set_level(int level)
{
	ko_log_level = level;
}

int ko_log_set_path( const char* path )
{
	strcpy(path_str,path);
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

	ko_log_app_fd = app_fd;
	ko_log_app_size = 0;
	
	return 0;
}



int ko_log_real( int wfd, int pos, int level, const char* format, va_list arg_list )
{
	char 			buffer[KO_LOG_BUFFER_DEFAULT];
	char 			*p;
	int 			size;
	
	p = buffer;
	size = sprintf(p,"[%s][%s]", get_time_str(true), ko_level_str[level]);
	p += size;
	size += vsprintf(p , format, arg_list);
	if(size >= KO_LOG_BUFFER_DEFAULT){
		printf("log buf is too big!");
        buffer[KO_LOG_BUFFER_DEFAULT - 1] = 0;
        printf("log buf:%s", buffer);
        return -1;
	}
	buffer[size] = '\n';
	size += 1;
	if( wfd ){
		if ( pwrite( wfd, buffer, size , pos) == -1  ){
			printf("pwrite error\n");
		}
		return size;
	}else{
		buffer[size] = 0;
		printf(buffer);
		return 0;
	}
}


#define ko_log_start(format ,level,wfd,pos) \
	va_list arg_list; \
	if(level > ko_log_level ){	\
		return 0;	\
	} \
	va_start (arg_list,format); \
	size = ko_log_real(wfd,pos,level,format,arg_list);
	
#define ko_log_end va_end( arg_list );

#define ko_log_check_sys \
	ko_log_sys_size += size;	\
	if(ko_log_sys_size > KO_LOG_SIZE_LIMIT){ \
		ko_log_turn_sys(); \
	}
	
#define ko_log_check_app \
	ko_log_app_size += size; \
	if(ko_log_app_size > KO_LOG_SIZE_LIMIT){ \
		ko_log_turn_app(); \
	}
		
	
int ko_log_sys_real_debug(  const char* format, ... )
{
	int size;
	ko_log_start( format, KO_LOG_LEVEL_DEBUG, ko_log_sys_fd, ko_log_sys_size);
	ko_log_end;
	ko_log_check_sys;
}
int ko_log_sys_real_info(  const char* format, ... )
{
	int size;
	ko_log_start( format, KO_LOG_LEVEL_INFO, ko_log_sys_fd, ko_log_sys_size);
	ko_log_end;
	ko_log_check_sys;
}
int ko_log_sys_real_warn(  const char* format, ... )
{
	int size;
	ko_log_start( format, KO_LOG_LEVEL_WARN, ko_log_sys_fd, ko_log_sys_size);
	ko_log_end;
	ko_log_check_sys;
}
int ko_log_sys_real_error(  const char* format, ... )
{
	int size;
	ko_log_start( format, KO_LOG_LEVEL_ERROR, ko_log_sys_fd, ko_log_sys_size);
	ko_log_end;
	ko_log_check_sys;
}


int ko_log_app_real_debug(  const char* format, ... )
{
	int size;
	ko_log_start( format, KO_LOG_LEVEL_ERROR, ko_log_app_fd, ko_log_app_size);
	ko_log_end;
	ko_log_check_app;
}
int ko_log_app_real_info(  const char* format, ... )
{
	int size;
	ko_log_start( format, KO_LOG_LEVEL_INFO, ko_log_app_fd, ko_log_app_size);
	ko_log_end;
	ko_log_check_app;
}
int ko_log_app_real_warn(  const char* format, ... )
{
	int size;
	ko_log_start( format, KO_LOG_LEVEL_WARN, ko_log_app_fd, ko_log_app_size);
	ko_log_end;
	ko_log_check_app;
}
int ko_log_app_real_error(  const char* format, ... )
{
	int size;
	ko_log_start( format, KO_LOG_LEVEL_ERROR, ko_log_app_fd, ko_log_app_size);
	ko_log_end;
	ko_log_check_app;
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