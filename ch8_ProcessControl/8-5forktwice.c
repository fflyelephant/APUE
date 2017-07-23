#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

/* 
	当你fork子进程后，在父进程中没有使用wait，而且还想再父进程结束之前不想让子进程成为僵尸进程，那么可以fork两次
	保证第一次fork的子进程先于第二次fork的子进程结束，这样第二次fork的子进程立即被init收养，不会产生僵尸进程。
*/
int main(int argc, char const *argv[])
{
	pid_t pid;
	if((pid = fork()) < 0){
		perror("first fork error");
		return -1;
	}else if(pid == 0){ 	/* 第一个子进程 */
		if((pid = fork()) < 0){
			perror("second fork error");
			return -1;
		}else if(pid > 0){/* 第二个fork的父进程也就是第一个子进程 */
			exit(0);
		}
		/* 这里是第二个子进程 */
		sleep(2);//确保第一个子进程先结束
		printf("second id:%d, father id:%d\n", getpid(), getppid());
		exit(0);
	}

	/* 第一个父进程 */
	
	if(waitpid(pid, NULL, 0) != pid)
	{
		perror("waitpid error");
		return -1;
	}
	return 0;
}
/*result:

	st@ubuntu:~/git_project/APUE/ch8_ProcessControl$ second id:17570, father id:1

*/