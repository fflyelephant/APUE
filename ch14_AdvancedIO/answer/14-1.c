#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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

/* 设置读锁 */
#define read_lock(fd, offset, whence, len)\
			lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
#define readw_lock(fd, offset, whence, len)\
			lock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))
/* 设置写锁 */
#define write_lock(fd, offset, whence, len)\
			lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
#define writew_lock(fd, offset, whence, len)\
			lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len))
/* 解锁 */
#define un_lock(fd, offset, whence, len)\
			lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))

void sig_int(int signo)
{
	;//只是为了唤醒pause()
}

int main(int argc, char const *argv[])
{
	int 	pid1, pid2, pid3;
	int 	fd;
	setbuf(stdout, NULL);
	signal(SIGINT, sig_int);

	if((fd = open("lockfile", O_RDWR|O_CREAT, 0666)) < 0){
		perror("open error");
		return -1;
	}

	/* 进程1获得文件读锁 */
	if((pid1 = fork()) < 0){
		perror("fork1 error");
		return -1;
	}else if(pid1 == 0){
		if(read_lock(fd, 0, SEEK_SET, 0) < 0){
			perror("read_lock 1 error");
			return -1;
		}
		printf("child1: got a read lock on file\n");
		pause();
		printf("child1: exit from pause\n");
		un_lock(fd, 0, SEEK_SET, 0);
		exit(0);
	}else{
		sleep(2);
	}

	/* 进程2获得文件读锁 */
	if((pid2 = fork()) < 0){
		perror("fork2 error");
		return -1;
	}else if(pid2 == 0){
		if(read_lock(fd, 0, SEEK_SET, 0) < 0){
			perror("read_lock 2 error");
			return -1;
		}
		printf("child2: got a read lock on file\n");
		pause();
		printf("child2: exit from pause\n");
		un_lock(fd, 0, SEEK_SET, 0);
		exit(0);
	}else{
		sleep(2);
	}	

	/* 进程3获取文件写锁 */
	if((pid3 = fork()) < 0){
		perror("fork3 error");
		return -1;
	}else if(pid3 == 0){
		if(writew_lock(fd, 0, SEEK_SET, 0) < 0)
			printf("child3: can't got write lock:%s\n", strerror(errno));
		else{
			printf("child3: got a writelock success!\n");
			printf("child3: exit from pause\n");
		}	
		pause();
		printf("child3: exit from pause\n");	
		un_lock(fd, 0, SEEK_SET, 0);		
		exit(0);
	}else{
		sleep(2);
	}	


	/* 查看如果一个获取写锁被阻塞时,后面的获取读锁是否能成功 */
	if(read_lock(fd, 0, SEEK_SET, 0) < 0)
		printf("last child: can't got read lock:%s\n", strerror(errno));
	else
		printf("after writelock pending, the readlock can also got!!\n");
	un_lock(fd, 0, SEEK_SET, 0);
	
	printf("kill child1...\n");
	kill(pid1, SIGINT);
	printf("kill child2...\n");
	kill(pid2, SIGINT);
	printf("kill child3...\n");
	kill(pid3, SIGINT);
	return 0;
}
/*
stone@cdWSCMPL07:~/test_my/github_test/APUE/ch14_AdvancedIO/answer$ ./a.out 
child1: got a read lock on file
child2: got a read lock on file
...	...	--> 写锁阻塞
after writelock pending, the readlock can also got!! --> 后续读锁仍然可获取
kill child1...
kill child2...
kill child3...
child1: exit from pause
child2: exit from pause
	--> child1 & child2 将读锁unlock之后,这时写锁才能获取
child3: got a writelock success!
child3: exit from pause

1:由结果来看,写锁阻塞之后,后面的读锁仍然可以获取到,这个线程之间的读写锁有一些区别.

*/