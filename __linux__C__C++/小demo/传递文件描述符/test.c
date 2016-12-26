#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
 
#include <sys/socket.h>
 
// CONTROLLEN 为cmsghdr加一个文件描述符的长度
#define CONTROLLEN CMSG_LEN(sizeof(int))
 
int send_fd(int fd, int fd_to_send) {
    struct iovec    iov[1];
    struct msghdr   msg;
    char        buf[1];         // buf用于表示传递的描述符是否合法，合法buf[0]=0, 否则buf[0]=1
    struct cmsghdr  *cmptr = NULL;
 
    iov[0].iov_base = buf;     
    iov[0].iov_len  = 1;       
    msg.msg_iov = iov;      // array of IO buffers
    msg.msg_iovlen  = 1;        // number of elements in array
    msg.msg_name    = NULL;
    msg.msg_namelen = 0;
 
    if (fd_to_send < 0) {
        msg.msg_control     = NULL;
        msg.msg_controllen  = 0;
        buf[0] = 1;
    } else {
        // cmsghdr 包含了要传递的信息
        if ((cmptr = malloc(CONTROLLEN)) == NULL) {
            return -1;
        }
        cmptr->cmsg_level = SOL_SOCKET;
        cmptr->cmsg_type  = SCM_RIGHTS;
        cmptr->cmsg_len    = CONTROLLEN;
        msg.msg_control   = cmptr;
        msg.msg_controllen= CONTROLLEN;
        *(int*)CMSG_DATA(cmptr) = fd_to_send;
        buf[0] = 0;
    }
     
    if (sendmsg(fd, &msg, 0) != 1) {
        return -1;
    }
    return 0;
}
 
int recv_fd(int fd, int *fd_to_recv) {
    int         nr;
    char        buf[1];
    struct iovec    iov[1];
    struct msghdr   msg;
    struct cmsghdr  *cmptr = NULL;
     
    iov[0].iov_base = buf;
    iov[0].iov_len  = 1;
    msg.msg_iov = iov;
    msg.msg_iovlen  = 1;
    msg.msg_name    = NULL;
    msg.msg_namelen = 0;
 
    if ((cmptr = malloc(CONTROLLEN)) == NULL) {
        return -1;
    }
    msg.msg_control = cmptr;
    msg.msg_controllen = CONTROLLEN;
 
    if(recvmsg(fd, &msg, 0) < 0) {
        printf("recvmsg error\n");
        return -1;
    }
 
    if(msg.msg_controllen < CONTROLLEN) {
        printf("recv_fd get invalid fd\n");
        return -1;
    }
     
    *fd_to_recv = *(int*)CMSG_DATA(cmptr);
    return 0;
}
 
int main() {
    int fd;
    pid_t   pid;
    int sockpair[2];
    int status;
    char    fname[256];
 
    status = socketpair(AF_UNIX, SOCK_STREAM, 0, sockpair);
    if (status < 0) {
        printf("Call socketpair error, errno is %d\n", errno);
        return errno;
    }
 
    pid = fork();
    if (pid == 0) {
        close(sockpair[1]);
         
        status = recv_fd(sockpair[0], &fd);
        if (status != 0) {
            printf("[CHILD]: recv error, errno is %d\n", status);
            return status;
        }
 
        status = write(fd, "Yao DengDeng", strlen("Yao Dengdeng"));
        if (status < 0) {
            printf("[CHILD]: write error, errno is %d\n", status);
            return -1;
        } else {
            printf("[CHILD]: append logo successfully\n");
        }
        close(fd);
 
        exit(0);
    }
     
    printf("[PARENT]: enter the filename:\n");
    scanf("%s", fname);
 
    fd = open(fname, O_RDWR | O_APPEND);
    if (fd < 0) {
        printf("[PARENT]: open file error, errno is %d\n", errno);
        return -1;
    }
 
    status = send_fd(sockpair[1], fd);
    if (status != 0) {
        printf("[PARENT]: send_fd error, errno is %d\n", status);
        return -1;
    }
    close(fd);
 
    wait(NULL);
    return 0;
}