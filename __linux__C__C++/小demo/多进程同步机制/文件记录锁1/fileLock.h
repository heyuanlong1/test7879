#ifndef fileLock_H_2015_09_18
#define fileLock_H_2015_09_18


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
/*

struct flock{
	short l_type; //F_RDLCK,F_WRLCK, or F_UNLCK
	off_t l_start; //offset in bytes, relative to l_whence
	short l_whence; //SEEK_SET, SEEK_CUR or SEEK_END
	off_t l_len; //length in bytes; 0 means lock to EOF
	pid_t l_pid; //returned with F_GETLK
};

*/

class fileLock
{
public:
	fileLock()
	{
		fd = -1;
	}
	int init(const char *path)
	{
		if ((fd = open(path, O_CREAT | O_RDWR, 0666)) < 0) 
           	return -1;
       	return 0;
	}
	~fileLock()
	{	
		if(fd != -1){
			close(fd);
			fd = -1;
		}
	}

	int read_w_lock()
	{
	    if (fd < 0)
	        return -1;
	    
	    struct flock lock;
	    lock_init(&lock, F_RDLCK, SEEK_SET, 0, 0);

	    if (fcntl(fd, F_SETLKW, &lock) != 0)
	        return -1;
   
	    return 0;
	}
	int write_w_lock()
	{
	    if (fd < 0)
	        return -1;

	    struct flock lock;
	    lock_init(&lock, F_WRLCK, SEEK_SET, 0, 0);

	    if (fcntl(fd, F_SETLKW, &lock) != 0)
	        return -1;

	    return 0;
	}
	int read_lock()
	{
	    if (fd < 0)
	        return -1;
	    
	    struct flock lock;
	    lock_init(&lock, F_RDLCK, SEEK_SET, 0, 0);

	    if (fcntl(fd, F_SETLK, &lock) != 0)
	        return -1;
   
	    return 0;
	}
	int write_lock()
	{
	    if (fd < 0)
	        return -1;

	    struct flock lock;
	    lock_init(&lock, F_WRLCK, SEEK_SET, 0, 0);

	    if (fcntl(fd, F_SETLK, &lock) != 0)
	        return -1;

	    return 0;
	}

	int unlock()
	{
	    if (fd < 0)
	        return -1;

	    struct flock lock;
	    lock_init(&lock, F_UNLCK, SEEK_SET, 0, 0);

	    if (fcntl(fd, F_SETLKW, &lock) != 0)
	        return -1;

	    return 0;
	}

	pid_t lock_test(struct flock *plock)
	{
	    lock_init(plock, F_WRLCK, SEEK_SET, 0, 0);

	    if (fcntl(fd, F_GETLK, plock) == -1)
	        return -1;


	    if(plock->l_type == F_UNLCK)
	        return 0;
	    return plock->l_pid;						//拿到此锁的pid
	}

	void print_lock(struct flock lock)
	{
	        printf(" -----------------------------\n");

	        if (lock.l_type == F_RDLCK) {
	                printf("\tl_type: F_RDLCK\n");
	        }
	        else if (lock.l_type == F_WRLCK) {
	                printf("\tl_type: F_WRLCK\n");
	        }
	        else if (lock.l_type == F_UNLCK) {
	                printf("\tl_type: F_UNLCK\n");
	        }

	        printf("\tl_start: %d\n", (int)lock.l_start);

	        if (lock.l_whence == SEEK_SET) {
	                printf("\tl_whence: SEEK_SET\n");
	        }
	        else if (lock.l_whence == SEEK_CUR) {
	                printf("\tl_whence: SEEK_CUR\n");
	        }
	        else if (lock.l_whence == SEEK_END) {
	                printf("\tl_whence: SEEK_END\n");
	        }

	        printf("\tl_len: %d\n", (int)lock.l_len);

	        printf(" -----------------------------\n");
	}


private:
	void lock_init(flock *lock, short type, short whence, off_t start, off_t len)
	{
	    if (lock == NULL)
	        return;

	    lock->l_type = type;
	    lock->l_whence = whence;
	    lock->l_start = start;
	    lock->l_len = len;
	}

private:
	int fd;
};


#endif