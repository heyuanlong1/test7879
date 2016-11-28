#include "htool.h"


//ok
void msSleep(int millSec)
{
#if (defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS_))
	Sleep(millSec); //#include <windows.h>
#else
	usleep(1000 * millSec);
#endif
}

//ok
long getFileSize(const char *path)
{
	long len= 0;
	FILE* file = fopen(path,"rb");
	if(NULL == file){
		return -1;
	}
	fseek(file,0L,SEEK_END);
	len = ftell(file);
	fclose(file);
	return len;

}

//ok
int checkIsDir(const char *path)
{
	struct stat st;
	if(stat(path,&st) != 0){
		return -1;
	}
	if(st.st_mode & S_IFDIR){
		return 0;
	}
	return -1;
}

//ok
int checkIsFile(const char *path)
{
	struct stat st;
	if(stat(path,&st) != 0){
		return -1;
	}
	if(st.st_mode & S_IFREG){
		return 0;
	}
	return -1;
}

//ok
//gethostbyname存在性能瓶颈，而却不能用于多线程。
int getIPStrByHost(const char * hostName,char *IPStr,int len)
{
	char **pptr = NULL;
	struct hostent *hptr  = gethostbyname(hostName);
	if(!hptr){
		return -1;
	}
	switch(hptr->h_addrtype){
		case AF_INET:
		case AF_INET6:
			pptr = hptr->h_addr_list;
			for (; pptr != NULL; ++pptr)
			{
				if(inet_ntop(hptr->h_addrtype,*pptr,IPStr,len) == NULL){
					return -1;
				}
				return 0;
			}
		default:
			return -1;
	}
	return -1;
}

void logInfo(char *fmt,...)
{	
	//#include <stdarg.h>
	static char sMessage[2048] = {0};
	va_list argp;
	va_start(argp,fmt);
	vsnprintf(sMessage,sizeof(sMessage),fmt,argp);
	va_end(argp);

	fprintf(stdout, "%s\n", sMessage);
	fflush(stdout);                     //不fflush则不会立即到文件里
}
void logError(char *fmt,...)
{
	#define LOG_MESSAGE 2048
	static char s_message[LOG_MESSAGE];
	static int fd = STDOUT_FILENO;
	int len;
	va_list argp;
	va_start(argp, fmt);
	len = vsnprintf(s_message, LOG_MESSAGE, fmt, argp);//返回字符串的长度
	va_end(argp);
	write(fd, s_message, len);
}