#include "log.h"
#include "conf/conf.h"
#define LOG_MESSAGE 2048

void log_error_fd(const char *fmt, ...)
{

	static char s_message[LOG_MESSAGE];
#if defined(HVS_DEBUG)
	static int fd = STDERR_FILENO;
#else
	static int fd = get_error_fd();
#endif

	int len;
	va_list argp;
	va_start(argp, fmt);
	len = vsnprintf(s_message, LOG_MESSAGE, fmt, argp);
	va_end(argp);
	write(fd, s_message, len);
}

void log_warning_fd(const char *fmt, ...)
{
	static char s_message[LOG_MESSAGE];
#if defined(HVS_DEBUG)
	static int fd = STDERR_FILENO;
#else
	static int fd = get_warning_fd();
#endif
	
	int len;
	va_list argp;
	va_start(argp, fmt);
	len = vsnprintf(s_message, LOG_MESSAGE, fmt, argp);
	va_end(argp);
	write(fd, s_message, len);
}

void log_info_fd(const char *fmt, ...)
{
	static char s_message[LOG_MESSAGE];
#if defined(HVS_DEBUG)
	static int fd = STDERR_FILENO;
#else
	static int fd = get_info_fd();
#endif

	int len;
	va_list argp;
	va_start(argp, fmt);
	len = vsnprintf(s_message, LOG_MESSAGE, fmt, argp);
	va_end(argp);
	write(fd, s_message, len);
}
