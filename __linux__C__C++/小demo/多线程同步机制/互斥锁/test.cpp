#include <stdio.h>
#include <stdlib.h>
#include "CommonMutex.h"

#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define NUMS  20000
CommonMutex lock;
int nums=0;

void * thrd_func(void *arg)
{
	while(1)
	{
		lock.lock();
		++nums;
		if (nums > NUMS){
			lock.unlock();
			pthread_exit(0);
		}	
		printf("in child thread %d\n",nums );
		lock.unlock();
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


	while(1)
	{
		AutoLocker obj(&lock);
		++nums;
		if (nums > NUMS)
			break;
		printf("in main thread %d\n",nums );

	}
	return 0;
}
