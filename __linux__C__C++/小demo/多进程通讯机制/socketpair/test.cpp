
/* pipe.c */

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> 

#define MAX_DATA_LEN	256
#define DELAY_TIME	1

int main()
{
	pid_t pid;
	int pair_fd[2];
	char buf[MAX_DATA_LEN];
	const char data1[] = "1 Test Program";
	const char data2[] = "2 Test Program";
	int real_read, real_write;
	
	memset((void*)buf, 0, sizeof(buf));
	
	if (socketpair(AF_UNIX,SOCK_STREAM,0, pair_fd) < 0){
		printf("sockpair create error\n");
		exit(1);
	}
	
	/* 创建一子进程 */
	if ((pid = fork()) == 0)
	{
		//close(pair_fd[1]);
		while(1){
			sleep(DELAY_TIME );
			if ((real_read = read(pair_fd[0], buf, MAX_DATA_LEN)) > 0){
				printf("child %d bytes read from the sockpair is '%s'\n", real_read, buf);
			}
			write(pair_fd[0], data1, strlen((const char*)data1));
		}
		
		close(pair_fd[0]);
		close(pair_fd[1]);
		exit(0);
	}
	else if (pid > 0)
	{
		//close(pair_fd[0]);
		while(1){
			sleep(DELAY_TIME * 2);
			 write(pair_fd[1], data2, strlen((const char*)data2));
			 if ((real_read = read(pair_fd[1], buf, MAX_DATA_LEN)) > 0){
				printf("parent %d bytes read from the sockpair is '%s'\n", real_read, buf);
			}
		}
		
		close(pair_fd[0]);
		close(pair_fd[1]);
		/*收集子进程退出信息*/
		waitpid(pid, NULL, 0);
		exit(0);
	}
}