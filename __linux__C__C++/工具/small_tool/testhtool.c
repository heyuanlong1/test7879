#include "htool.h"

int main(int argc, char const *argv[])
{
	const int ipstrlen=64;
	char ipstr[64]={0};


	printf("msSleep\n");
	msSleep(300);

	printf("getFileSize\n");
	printf("%d\n", getFileSize("/home/heyuanlong/ctesta/htool.h"));

	printf("checkIsDir\n");
	printf("%d\n", checkIsDir("/home/heyuanlong/"));

	printf("checkIsFile\n");
	printf("%d\n", checkIsFile("/home/heyuanlong/ctesta/htool.h"));


	printf("getIPStrByHost:cn.matchvs.com\n");
	printf("%d\n", getIPStrByHost("cn.matchvs.com",ipstr,ipstrlen));
	printf("%s\n",ipstr );

	printf("logInfo\n");
	logInfo("%d,%s",45,"dddddddddd");
	printf("logError\n");
	logError("%d,%s\n",45,"sss");

	return 0;
}