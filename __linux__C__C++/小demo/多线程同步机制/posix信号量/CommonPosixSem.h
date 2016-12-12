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
		return sem_post(&s);
	}
	int wait()
	{
		return sem_wait(&s);
	}
	int strwait()
	{
		return sem_trywait(&s);
	}
	sem_t* getSem()
	{
		return &s;
	}

public:
	sem_t s;
};


#endif