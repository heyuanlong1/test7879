#include <stdio.h>
#include <stdlib.h>
#include "systemVSem.h"

#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define NUMS  1000000

int main(int argc, char const *argv[])
{
	systemVSem s(ftok(".", 'a'),1);

	int result = fork();
	
	/*通过result的值来判断fork函数的返回情况，首先进行出错处理*/
	if(result ==  -1)
	{
		perror("Fork\n");
	}
	else if (result == 0) /*返回值为0代表子进程*/
	{
		while(1){
			s.sem_p(0);
			printf("Child process...............\n");
			usleep(NUMS);
			s.sem_v(0);
		}
		
	}
	else /*返回值大于0代表父进程*/
	{
		while(1){
			s.sem_p(0);
			printf("father process...\n");
			usleep(100);
			s.sem_v(0);
		}
	}
	return 0;
}
