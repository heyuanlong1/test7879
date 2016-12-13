#include <stdio.h>
#include <stdlib.h>
#include "systemVSem.h"

#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define NUMS  (1000000)

int main(int argc, char const *argv[])
{
	systemVSem s;
	if(s.init(key_t(1234),1) < 0)
	{
		perror("");
		return -1;
	}
	s.sem_p(0);

	while(1){
		s.sem_p(0);
		printf("test3 process...\n");
		usleep(NUMS / 2);
		s.sem_v(0);
	}
	
	return 0;
}
