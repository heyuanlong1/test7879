#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

/*自定义信号处理函数*/
void my_func(int sign_no)
{
	if (sign_no == SIGINT)
	{
		printf("I have get SIGINT\n");
	}	
  	else if (sign_no == SIGQUIT)
  	{
  		printf("I have get SIGQUIT\n");
  	}
}

int main()
{
	printf("Waiting for signal SIGINT or SIGQUIT...\n");
	
	/* 发出相应的信号，并跳转到信号处理函数处 */
	signal(SIGINT, my_func);//设置信号相对应的函数,signal是只向本进程发送信号的
	signal(SIGQUIT, my_func);
	signal(SIGTSTP, SIG_IGN);//忽略信息

	//raise(SIGINT);//也可以使用ctrl + c 触发信号
	//raise(SIGQUIT);//也可以使用ctrl + \ 触发此信号
	//raise(SIGTSTP);//也可以使用ctrl + z 触发此信号
    while(1)  
    {  
         sleep(1);  
    }  
	return 0;
}