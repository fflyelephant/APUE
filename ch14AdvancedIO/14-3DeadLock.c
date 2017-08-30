#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


/* 锁和解锁记录锁 */
int	lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
	struct flock 	lock;
	lock.l_type 	= type;
	lock.l_whence	= whence;
	lock.l_start	= offset;
	lock.l_len		= len;
	return(fcntl(fd, cmd, &lock));
}
#define writew_lock(fd, offset, whence, len)\
			lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len))

static void lockbyte(const char *name, int fd, off_t offset)
{
	if(writew_lock(fd, offset, SEEK_SET, 1) < 0){
		perror("writew_lock error");
		return;
	}
	printf("%s:got a lock, byte %lld\n", name, (long long)offset);
}

int main(int argc, char const *argv[])
{
	int 	fd;
	pid_t	pid;

	if((fd = open("./templock", O_RDWR|O_CREAT)) < 0){
		perror("open error");
		return -1;
	}

	if(write(fd, "ab", 2) != 2){
		perror("write error");
		return -1;
	}

	if((pid = fork()) < 0){
		perror("fork error");
		return -1;
	}else if(pid == 0){
		sleep(2);//让父进程先锁住
		//lockbyte("child", fd, 0);
		printf("%d\n", __LINE__);
		lockbyte("child", fd, 1);
		printf("%d\n", __LINE__);
		return 0;
	}else{
		lockbyte("father", fd, 1);
		lockbyte("father", fd, 0);
		printf("%d\n", __LINE__);
		lockbyte("father", fd, 1);
		lockbyte("father", fd, 0);	
		printf("%d\n", __LINE__);
	}
	wait(NULL);
	return 0;
}
/*
stone@cdWSCMPL07:~/test_my/github_test/APUE/ch14AdvancedIO$ ./a.out 
father:got a lock, byte 1
father:got a lock, byte 0
60
father:got a lock, byte 1  --> 说明同一进程下的记录锁不存在死锁情况
father:got a lock, byte 0
63
53
--> 子进程死锁阻塞
^C
*/