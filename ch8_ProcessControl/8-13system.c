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
#include <errno.h>
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

int mysystem(const char *cmd)
{
	pid_t pid;
	int status;
	if(cmd == NULL)
		return 1;

	if((pid = fork()) < 0){
		perror("fork error");
		return -1;
	}
	else if(pid == 0){
		execl("/bin/sh", "sh", "-c", cmd, (char *)0);
		_exit(127);/* execl error 不用exit防止IO缓冲(io缓冲会从父进程拷贝到子进程中来) */
	}
	else{
		while(waitpid(pid, &status, 0) < 0)
			if(errno != EINTR){
				status = -1;
				break;
			}
	}
	return status;
}

int main(int argc, char const *argv[])
{
	int status;
	if((status = mysystem("ls -l")) < 0)
	{
		perror("first mysystem error");
		return -1;
	}
	pr_exit(status);

	if((status = mysystem("errorcmd")) < 0)
	{
		perror("second mysystem error");
		return -1;
	}
	pr_exit(status);

	if((status = mysystem("who; exit 66")) < 0)
	{
		perror("third mysystem error");
		return -1;
	}
	pr_exit(status);
	return 0;
}
/*result:

	st@ubuntu:~/git_project/APUE/ch8_proctl$ ./a.out 
	total 32 											//ls -l 命令
	-rwxrwxrwx 1 st st  519 Jul  9 17:12 8-12compete.c
	-rwxrwxrwx 1 st st  441 Jul  9 22:30 8-17echoall.c
	-rwxrwxrwx 1 st st 1664 Jul 10 21:03 8-17exec.c
	-rwxrwxrwx 1 st st 1968 Jul 10 22:11 8-23system.c
	-rwxrwxrwx 1 st st 1978 Jul 10 21:58 8-6wait.c
	-rwxrwxrwx 1 st st  519 Jul 10 21:04 8-9compete.c
	-rwxrwxrwx 1 st st 7450 Jul 10 22:12 a.out
	normal termination, exit status=0
	sh: 1: errorcmd: not found							//errorcmd 命令
	normal termination, exit status=127					//who 命令
	st       tty7         2017-07-07 21:15
	st       pts/0        2017-07-07 21:16 (:0)
	normal termination, exit status=66

*/