#include <sys/types.h>		/* some systems still require this */
#include <sys/stat.h>
#include <sys/termios.h>	/* for winsize */
#include <sys/ioctl.h>
#include <stdio.h>		/* for convenience */
#include <stdlib.h>		/* for convenience */
#include <stddef.h>		/* for offsetof */
#include <string.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
#include <signal.h>		/* for SIG_ERR */
#include <sys/wait.h>	/* for WIFXXX */

/*
	pr_exit函数打印进程的退出状态
	WIFXXX宏得到子进程返回值的状态，判断出退出方式。
*/
void pr_exit(int status)
{
	if(WIFEXITED(status))//正常退出
		printf("normal termination, exit status=%d\n", WEXITSTATUS(status));
	else if(WIFSIGNALED(status))//异常终止
		printf("abnormal termination, signal number=%d, %s\n", 
			WTERMSIG(status), 
#ifdef WCOREDUMP
			WCOREDUMP(status) ? "got a core file":" no core file"
#endif	
			);
	else if(WIFSTOPPED(status))//子进程停止
		printf("child was stopped, signal number=%d\n", WSTOPSIG(status));
}


int main(int argc, char const *argv[])
{
	pid_t pid;
	int status;
	/*子进程return终止*/
	if((pid = fork()) < 0)
		printf("fork error\n");
	else if(pid == 0)
		return 0;
	if(wait(&status) != pid)
		printf("wait error\n");
	pr_exit(status);

	/*子进程exit终止*/
	if((pid = fork()) < 0)
		printf("fork error\n");
	else if(pid == 0)
		exit(7);
	if(wait(&status) != pid)
		printf("wait error\n");
	pr_exit(status);

	/*子进程异常退出，收到SIGABRT信号*/
	if((pid = fork()) < 0)
		printf("fork error\n");
	else if(pid == 0)
		abort();
	if(wait(&status) != pid)
		printf("wait error\n");
	pr_exit(status);

	/*子进程异常退出，内核产生相应信号*/
	if((pid = fork()) < 0)
		printf("fork error\n");
	else if(pid == 0)
		status /= 0;
	if(wait(&status) != pid)
		printf("wait error\n");
	pr_exit(status);


	return 0;
}

/*result:

	st@ubuntu:~/git_project/APUE/ch8_ProcessControl$ ./a.out 
	normal termination, exit status=0
	normal termination, exit status=7
	abnormal termination, signal number=6, got a core file
	abnormal termination, signal number=8, got a core file

*/