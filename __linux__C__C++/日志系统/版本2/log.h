#ifndef __Log_h__
#define __Log_h__


#ifndef KO_LOG_PATH
#define KO_LOG_PATH				"/tmp"
#endif


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




int ko_log_sys_real_debug( format, ... )
int ko_log_sys_real_info( format, ... )
int ko_log_sys_real_warn( format, ... )
int ko_log_sys_real_error( format, ... )


int ko_log_app_real_debug( format, ... )
int ko_log_app_real_info( format, ... )
int ko_log_app_real_warn( format, ... )
int ko_log_app_real_error( format, ... )


int ko_log_init();
int ko_log_set_path( const char* path );
int ko_log_set_path_for_printf();
int ko_log_close();




#endif // __Log_h__