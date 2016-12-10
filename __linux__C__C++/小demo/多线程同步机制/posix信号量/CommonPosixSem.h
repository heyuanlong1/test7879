#ifndef COMMONPOSIXSEM_H_2015_09_18
#define COMMONPOSIXSEM_H_2015_09_18


#include <pthread.h>
#include <semaphore.h>

class CommonPosixSem
{
public:
	CommonPosixSem()
	{
		sem_init(&s,0,0);
	}
	~CommonPosixSem()
	{
		sem_destroy(&s);
	}
	int post()
	{
		sem_post(&s);
	}
	int wait()
	{
		sem_wait(&s);
	}
	int strwait()
	{
		sem_trywait(&s);
	}
	sem_t* getmutex()
	{
		return &s;
	}

public:
	sem_t s;
};


#endif