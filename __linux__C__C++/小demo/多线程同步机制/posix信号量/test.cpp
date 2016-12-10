#include <stdio.h>
#include <stdlib.h>
#include "CommonPosixSem.h"

#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define NUMS  1000000
CommonPosixSem sem;
int nums=0;

void * thrd_func(void *arg)
{
	while(1)
	{
		sem.wait();
		printf("wait\n");
	}
}

int main(int argc, char const *argv[])
{

	pthread_t m_threadId;
	pthread_attr_t attr;//线程属性变量
	int res = 0;
	
	res = pthread_attr_init(&attr);//初始化线程属性
	if (res != 0){
		printf("Create attribute failed\n");
		exit(res);
	}
	res += pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);//设置分离属性
	if (res != 0){
		printf("Setting attribute failed\n");
		exit(res);
	}
	if (0 != pthread_create(&m_threadId, &attr, thrd_func, NULL))
		return -1;

	usleep(10000);
	while(1)
	{
		sem.post();
		printf("post\n");
		usleep(NUMS);
	}
	return 0;
}
