#ifndef __SERVER__
#define __SERVER__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>
#include <list>
#include "mysql.h"
#include "mysqlPool.h"
#include "mutex.h"

class controlUnit
{
public:
	controlUnit();
	~controlUnit();
	void push_back(int fd);
	void remove(int fd);

	bool empty();
	void lock();
	void unlock();
	pthread_mutex_t * getMutex();
	pthread_cond_t * getCond();
	void setNext(controlUnit* n);
	controlUnit * getNext();

	int getReadPipe();
	int getWritePipe();
	int getArrFd(int *arrFd,int *arrNums);
private:
	int pipeFd[2];
	std::list<int> fdList;
	CommonMutex mutex;
	pthread_cond_t haveProduct;
	controlUnit* next;
	/* data */
};

#endif