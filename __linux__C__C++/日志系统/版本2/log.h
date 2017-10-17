#ifndef __Log_h__
#define __Log_h__


#ifndef KO_LOG_PATH
#define KO_LOG_PATH				"/tmp"
#endif

#define KO_LOG_BUFFER_DEFAULT			2048
#define KO_LOG_SIZE_LIMIT       		104857600			//100 * 1024 * 1024static int ko_log_error_fd = 0;
#define MAX_TIME_STR_LEN    			30
typedef unsigned long uint32;

enum {
    KO_LOG_LEVEL_ERROR     = 0,  //错误
    KO_LOG_LEVEL_WARN	   = 1,  //警告
    KO_LOG_LEVEL_INFO      = 2,  //普通
    KO_LOG_LEVEL_DEBUG     = 3,  //调试
    KO_LOG_LEVEL_MAX	   = 4,	 // 
};
static const char* ko_level_str[] = {"ERROR", "WARNING", "INFO", "DEBUG", "MAX"};



//系统日志
#define ko_log_sys_debug( format, args... )				ko_log_sys_real_debug( format, ##args )
#define ko_log_sys_info( format, args... )				ko_log_sys_real_info( format, ##args )
#define ko_log_sys_warn( format, args... )				ko_log_sys_real_warn( format, ##args )
#define ko_log_sys_error( format, args... )				ko_log_sys_real_error( format, ##args )

//应用日志
#define ko_log_debug( format, args... )					ko_log_app_real_debug( format, ##args )
#define ko_log_info( format, args... )					ko_log_app_real_info( format, ##args )
#define ko_log_warn( format, args... )					ko_log_app_real_warn( format, ##args )
#define ko_log_error( format, args... )					ko_log_app_real_error( format, ##args )




int ko_log_sys_real_debug( const char* format, ... );
int ko_log_sys_real_info( const char*  format, ... );
int ko_log_sys_real_warn(  const char* format, ... );
int ko_log_sys_real_error( const char*  format, ... );


int ko_log_app_real_debug( const char*  format, ... );
int ko_log_app_real_info(  const char* format, ... );
int ko_log_app_real_warn(  const char* format, ... );
int ko_log_app_real_error(  const char* format, ... );


int ko_log_init();
int ko_log_set_level(int level);
int ko_log_set_path( const char* path );
int ko_log_set_path_for_printf();
int ko_log_close();




#endif // __Log_h__