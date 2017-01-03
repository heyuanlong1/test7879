#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*自定义信号处理函数*/
void my_func(int sign_no)
{
	if (sign_no == SIGINT)
		printf("I have get SIGINT\n");
  	else if (sign_no == SIGQUIT)
  		printf("I have get SIGQUIT\n");
  	else
		printf("????\n");
}
void my_func2(int sign_no)
{
	if (sign_no == SIGINT)
		printf("I have get SIGINT\n");
  	else if (sign_no == SIGQUIT)
  		printf("I have get SIGQUIT\n");
  	else
		printf("SIGTSTP\n");
}


int main()
{
	struct sigaction action, action2;
	printf("Waiting for signal SIGINT or SIGQUIT...\n");
	
	/* sigaction结构初始化 */
	action.sa_handler = my_func;
	sigemptyset(&action.sa_mask);//清空信号集
	sigaddset(&action.sa_mask, SIGQUIT);
	//sa_mask，信号屏蔽集，可以通过函数sigemptyset/sigaddset等来清空和增加需要屏蔽的信号，上面代码中，对信号SIGINT处理时，如果来信号SIGQUIT，其将被屏蔽，但是如果在处理SIGQUIT，来了SIGINT，则首先处理SIGINT，然后接着处理SIGQUIT。
	action.sa_flags = 0;
	
	/* 如果发出相应的信号，并跳转到信号处理函数处 */
	sigaction(SIGINT, &action, 0);//,设置信号相对应的函数


	action2.sa_handler = my_func2;
	sigemptyset(&action2.sa_mask);//不屏蔽任何信号
	action2.sa_flags = 0;

	sigaction(SIGQUIT, &action2, 0);//,设置信号相对应的函数
	sigaction(SIGTSTP, &action2, 0);//,设置信号相对应的函数

	//raise(SIGINT);//也可以使用ctrl + c 触发信号
	//raise(SIGQUIT);//也可以使用ctrl + \ 触发此信号
	//raise(SIGTSTP);//也可以使用ctrl + z 触发此信号
	while(1)  
    {  
         sleep(1);  
    }  
	return 0;
}