
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
	int i;
	int shmid;
	char c,*shm_addr,*shm_addr1;
	struct shmid_ds ds_buff;
	char flag[] = "WROTE";
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
		
		shmctl(shmid,IPC_STAT,&ds_buff);
		printf("child shm_segsz:%d\n", ds_buff.shm_segsz);

		while (1)
		{
			shm_addr1 = shm_addr;		
			for(i=0;i < 5 ;i++)
		    {
		        c = *shm_addr1++;
		        printf("ptr[%d] = %c\n",i,c);
		    }
			sleep(2);
		}
		
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
		shmctl(shmid,IPC_STAT,&ds_buff);
		printf("parent shm_segsz:%d\n", ds_buff.shm_segsz);

		while(1){
			shm_addr1 = shm_addr;
			fgets(buff, BUFFER_SIZE, stdin);
			for(i=0; i < strlen(buff) ; i++)
	        	*shm_addr1++ = buff[i];
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
