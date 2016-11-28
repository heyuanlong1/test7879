#include "controlUnit.h"

controlUnit::controlUnit()
{
	if ((pipe(pipeFd)) < 0) {
		perror("pipe");
		exit(0);
	}
}

controlUnit::~controlUnit()
{
	close(pipeFd[0]);
	close(pipeFd[1]);
}


void controlUnit::lock()
{
	mutex.lock();
}

void controlUnit::unlock()
{
	mutex.unlock();
}

void controlUnit::push_back(int fd)
{
	fdList.push_back(fd);
}
void controlUnit::remove(int fd)
{
	fdList.remove(fd);
}

bool controlUnit::empty()
{
	return fdList.empty();
}

pthread_mutex_t * controlUnit::getMutex()
{
	return mutex.getmutex();
}

pthread_cond_t * controlUnit::getCond()
{
	return &haveProduct;
}

void controlUnit::setNext(controlUnit* n)
{
	next = n;
}

controlUnit * controlUnit::getNext()
{
	return next;
}

int controlUnit::getReadPipe()
{
	return pipeFd[0];
}

int controlUnit::getWritePipe()
{
	return pipeFd[1];
}

int controlUnit::getArrFd(int *arrFd,int *arrNums)
{
	std::list<int>::iterator b,e;
	b = fdList.begin();
	e = fdList.end();
	int i = 0;
	for ( ;b != e; ++b,++i){
		if (i>= *arrNums){
			break;
		}
		arrFd[i] = *b;
	}
	*arrNums = i + 1;
	return fdList.size();
}