#include <stdio.h>
#include <string.h>
#include <unistd.h>


int main()
{
 	int pipeFd[2];
 	if ((pipe(pipeFd)) < 0)
 	{
 		perror("pipe");
 		return 0;
 	}
 	int rr,ww;
 	rr = pipeFd[0];
 	ww = pipeFd[1];
 	write(ww,"123",strlen("123"));
 	char rbuf[100] = {0};
 	int len = read(rr,rbuf,100);
 	printf("%d\n", len);
 	char rrbuf[100] = {0};
 	len = read(rr,rrbuf,100);
 	printf("%d\n", len);
 	
   	return 0;
}
