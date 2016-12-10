#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/*
sigemptyset(&newmask);//获取空屏蔽信号集
sigfillset(&newmask);//获取屏蔽了所有信号的屏蔽信号集，除了那两个SIGKILL SIGSTOP
sigpending(&pendmask);//获取进程中当前的屏蔽信号集

sigaddset(&newmask, SIGQUIT);//往空屏蔽信号集加入SIGQUIT
(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)//并集
(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)//设置回原来的屏蔽信号集
(sigismember(&fillmask, SIGQUIT))//判断SIGQUIT是否在当前的屏蔽信号集中
*/

static void	sig_quit(int);

int
main(void)
{
	sigset_t	newmask, oldmask, pendmask,fillmask;

	if (signal(SIGQUIT, sig_quit) == SIG_ERR)//设置信号处理函数
		perror("can't catch SIGQUIT");

	/*
	 * Block SIGQUIT and save current signal mask.
	 */
	sigemptyset(&newmask);//获取空屏蔽信号集
	sigaddset(&newmask, SIGQUIT);//往空屏蔽信号集加入SIGQUIT
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)//并集
		perror("SIG_BLOCK error");

	sleep(5);	/* SIGQUIT here will remain pending */
    //睡眠期间按下多次 ctrl+\   未决信号集中之保留一次

	if (sigpending(&pendmask) < 0)//获取进程中当前的屏蔽信号集
		perror("sigpending error");
	if (sigismember(&pendmask, SIGQUIT))//判断SIGQUIT是否在当前的屏蔽信号集中
		printf("\nSIGQUIT pending\n");

	/*
	 * Restore signal mask which unblocks SIGQUIT.
	 */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)//设置会原来的屏蔽信号集，由于SIGQUIT保留着一个未决信号，所以这里触发了信号处理函数，SIGQUIT变回默认处理
		perror("SIG_SETMASK error");
	printf("SIGQUIT unblocked\n");

	sleep(5);	/* SIGQUIT here will terminate with core file */ //在此期间按下  ctrl+\ 执行信号处理函数 ，调用默认处理。game over！
	
	printf("fillmask test\n");

	if (sigfillset(&fillmask) < 0)//获取屏蔽了所有信号的屏蔽信号集，除了那两个SIGKILL SIGSTOP
		perror("sigfillset error\n");
	if (sigismember(&fillmask, SIGQUIT))//判断SIGQUIT是否在当前的屏蔽信号集中
		printf("\nSIGQUIT pending\n");//在
	if (sigprocmask(SIG_SETMASK,&fillmask , &oldmask) < 0)//设置屏蔽所有
			perror("fillmask error\n");

	sleep(5);
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)//如果在fillmask测试期间有按下ctrl+\，则下面不会睡眠5秒了，执行了SIGQUIT的默认处理函数
		perror("SIG_SETMASK error\n");

	while(1){
		sleep(1);
	}
	return 0;
}

static void
sig_quit(int signo)
{
	printf("caught SIGQUIT\n");
	//if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
	//	perror("can't reset SIGQUIT");
}
