#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

/*自定义的信号处理函数*/
void my_func(int signum)
{
	printf("If you want to quit,please try SIGQUIT\n");
}

int main()
{
	sigset_t set,pendset;
	struct sigaction action1,action2;
	
	sigemptyset(&set);				/* 初始化信号集为空 */
	sigaddset(&set, SIGQUIT);		/* 将相应的信号加入信号集 */
	sigaddset(&set, SIGINT);
	
	//sigismember查询指定的信号是否在信号集合中
	if (sigismember(&set, SIGINT))
	{
		sigemptyset(&action1.sa_mask);		// 处理信号时，不屏蔽任何信号
		action1.sa_handler = my_func;
		action1.sa_flags = 0;
		sigaction(SIGINT, &action1, NULL);	//,设置信号相对应的函数
	}
	
	if (sigismember(&set, SIGQUIT))
	{
		sigemptyset(&action2.sa_mask);		// 处理信号时，不屏蔽任何信号
		action2.sa_handler = SIG_DFL;
		action2.sa_flags = 0;
		sigaction(SIGQUIT, &action2,NULL);
	}


//----------------------------------
//屏蔽测试和解屏蔽测试
	/* 设置信号集屏蔽字，此时set中的信号不会被传递给进程，暂时进入待处理状态 */
	if (sigprocmask(SIG_BLOCK, &set, NULL) < 0){
		perror("sigprocmask");
		exit(1);
	}
	else{
		printf("Signal set was blocked, Press any key!");
		getchar();
	}
	
	/* 在信号屏蔽字中删除set中的信号 */
	if (sigprocmask(SIG_UNBLOCK, &set, NULL) < 0){
		perror("sigprocmask");
		exit(1);
	}
	else{
		printf("Signal set is in unblock state\n");
	}

//---------------------------------
	//raise(SIGINT);//也可以使用ctrl + c 触发信号
	//raise(SIGQUIT);//也可以使用ctrl + \ 触发此信号
	while(1){
		sleep(1);
	}
	return 0;
}