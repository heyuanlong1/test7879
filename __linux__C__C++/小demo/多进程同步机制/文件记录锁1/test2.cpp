#include <stdio.h>
#include <stdlib.h>
#include "fileLock.h"

#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define NUMS  (1000000)

int main(int argc, char const *argv[])
{
	fileLock s;
	if(s.init("./lockFile") < 0)
	{
		perror("");
		return -1;
	}
	printf("start\n");
	struct flock pf;
	printf("%d\n", s.lock_test(&pf)); 
	s.print_lock(pf);
	while(1){
		s.read_w_lock();
		printf("test2 process...............\n");
		usleep(NUMS);
		s.unlock();
	}
	
	return 0;
}
