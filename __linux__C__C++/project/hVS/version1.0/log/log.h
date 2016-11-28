#ifndef __LOG__H__
#define __LOG__H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void log_error_fd(const char *fmt, ...);
void log_warning_fd(const char *fmt, ...);
void log_info_fd(const char *fmt, ...);
#endif 