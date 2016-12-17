
/* shmem.c */

#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 2048

int main()
{
	pid_t pid;
	int shmid;
	char *shm_addr;
	char flag[] = "end";
	char buff[1024];
	
	/* 创建共享内存 */
	if ((shmid = shmget(IPC_PRIVATE, BUFFER_SIZE, 0666)) < 0){
		perror("shmget");
		exit(1);
	}

	/* 显示共享内存情况 */
	system("ipcs -m");
	
	pid = fork();
	if (pid == -1){
		perror("fork");
		exit(1);
	}
	else if (pid == 0) /* 子进程处理 */
	{
		/*映射共享内存*/
		if ((shm_addr = shmat(shmid, NULL, 0)) == (void*)-1){
			perror("Child: shmat");
			exit(1);
		}
		
		/* 通过检查在共享内存的头部是否标志字符串"WROTE"来确认父进程已经向共享内存写入有效数据 */
		sleep(5);
		while (strncmp(shm_addr, flag, strlen(flag)))
		{
			printf("Child: Wait for enable data...\n");
			strcpy(buff, shm_addr);
			printf("Child: Shared-memory :%s\n", buff);
			sleep(2);
		}
		
		/* 获取共享内存的有效数据并显示 */
		strcpy(buff, shm_addr + strlen(flag));
		printf("Child: Shared-memory :%s\n", buff);
		
		/* 解除共享内存映射 */
		shmdt(shm_addr);
	  	system("ipcs -m");
	  	shmctl(shmid, IPC_RMID, NULL);
	}
	else /* 父进程处理 */
	{
		/*映射共享内存*/
		if ((shm_addr = shmat(shmid, NULL, 0)) == (void*)-1){
			perror("Parent: shmat");
			exit(1);
		}
		
		while(1){
			printf("\nInput some string:\n");
			fgets(buff, BUFFER_SIZE, stdin);
			strncpy(shm_addr, buff, strlen(buff));//写入共享内存

			if( strncmp(shm_addr, flag, strlen(flag)) == 0 )
				break;
		}
		/* 解除共享内存映射 */
		shmdt(shm_addr);
		system("ipcs -m");
		shmctl(shmid, IPC_RMID, NULL);
		waitpid(pid, NULL, 0);
		printf("Finished\n");
	}

  	exit(0);
}
