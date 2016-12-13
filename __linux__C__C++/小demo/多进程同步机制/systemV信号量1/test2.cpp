#include <stdio.h>
#include <stdlib.h>
#include "systemVSem.h"

#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define NUMS  1000000

int main(int argc, char const *argv[])
{
	systemVSem s;
	if(s.init(key_t(1234),1) < 0)
	{
		perror("");
		return -1;
	}

	while(1){
		s.sem_p(0);
		printf("test2 process...............\n");
		usleep(NUMS);
		s.sem_v(0);
	}
	
	return 0;
}
