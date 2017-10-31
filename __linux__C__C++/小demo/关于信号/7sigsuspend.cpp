#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>


void handler(int signum)
{
	printf("one signo %d\n",signum);
}



int main(int argc, char *argv[])
{
    sigset_t old,set;   //三个信号集
    struct sigaction act;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);    //可以捕捉以下信号：SIGINT/SIGQUIT/
    sigaction(SIGQUIT, &act, 0);
   
    sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGQUIT);
	if (sigprocmask(SIG_BLOCK, &set, &old) == -1) {
        printf("sigprocmask fail\n");
    }
	printf("sigprocmask\n");
	
	
	sigemptyset(&set);
	if(sigsuspend(&set) != -1){				//这句导致上面的sigprocmask暂时失效，系统能接收任何信号
		printf("sigsuspend fail\n");
	}
	printf("sigsuspend\n");
	
	
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	if(sigsuspend(&set) != -1){				//这句导致系统屏蔽SIGINT，接收其他任何信号
		printf("sigsuspend2 fail\n");
	}
	printf("sigsuspend2\n");
    
	
	//sigprocmask又有效了，系统不能接收SIGINT和SIGQUIT信号。
	while(1){
		sleep(1);
	}
	return 0;
	
}

