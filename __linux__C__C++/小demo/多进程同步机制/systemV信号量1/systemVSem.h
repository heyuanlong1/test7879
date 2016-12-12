#ifndef systemVSem_H_2015_09_18
#define systemVSem_H_2015_09_18

#include <sys/ipc.h>
#include <sys/sem.h>

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

class systemVSem
{
public:
	systemVSem(key_t key,int nums)
	{
		if(nums == 0)
			nums = 1;

		this->nums = nums;
		sem_id = semget(key,  nums, 0666|IPC_CREAT); /* 创建一个信号量*/
		union semun sem_union;
		for (int i = 0; i < nums; ++i)
		{
			sem_union.val = 1;
			if (semctl(sem_id, i, SETVAL, sem_union) == -1)
				perror("Initialize semaphore");				
		}
	}
	~systemVSem()
	{	
		union semun sem_union;
		for (int i = 0; i < nums; ++i)
		{
			if (semctl(sem_id, i, IPC_RMID, sem_union) == -1)
				perror("Delete semaphore");
		}
	}
	int sem_p(int n)
	{
		struct sembuf sem_b;
		sem_b.sem_num = n; /*id*/
		sem_b.sem_op = -1; /* P operation*/
		sem_b.sem_flg = SEM_UNDO;
	
		if (semop(sem_id, &sem_b, 1) == -1) {
			perror("P operation");
			return -1;
		}
		return 0;
	}

	int sem_v(int n)
	{
		struct sembuf sem_b;
		sem_b.sem_num = n; /*id*/
		sem_b.sem_op = 1; /* P operation*/
		sem_b.sem_flg = SEM_UNDO;
	
		if (semop(sem_id, &sem_b, 1) == -1) {
			perror("V operation");
			return -1;
		}
		return 0;		
	}

public:
	int sem_id;
	int nums;
};


#endif