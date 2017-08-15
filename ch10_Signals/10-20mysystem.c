#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

static int mysystem(const char *cmdstr)
{
	pid_t 				pid;
	int 				status;
	struct  sigaction	ignore, saveint, savequit;
	sigset_t			chldmask, savemask;

	if(cmdstr == NULL)
		return 1;

	/* 忽略SIGINT和SIGQUIT信号 */
	ignore.sa_handler = SIG_IGN;
	sigemptyset(&ignore.sa_mask);
	ignore.sa_flags = 0;
	if(sigaction(SIGINT, &ignore, &saveint) < 0){
		perror("sigaction SIGINT error");
		return -1;
	}
	if(sigaction(SIGQUIT, &ignore, &savequit) < 0){
		perror("sigaction SIGQUIT error");
		return -1;
	}

	/* 进程阻塞SIGCHLD信号 */
	sigemptyset(&chldmask);
	sigaddset(&chldmask, SIGCHLD);
	if(sigprocmask(SIG_BLOCK, &chldmask, &savemask) < 0){
		perror("sigprocmask error");
		return -1;
	}

	if((pid = fork()) < 0){
		status = -1;
	}else if(pid == 0){
		/* 子进程在执行新程序前恢复SIGINT和SIGQUIT的处理方式以及进程的信号集(这些会被execl执行的新进程继承) */
		sigaction(SIGINT, &saveint, NULL);
		sigaction(SIGQUIT, &savequit, NULL);
		sigprocmask(SIG_SETMASK, &savemask, NULL);

		execl("/bin/sh", "sh", "-c", cmdstr, (char*)0);
		_exit(127);
	}else{
		while(waitpid(pid, &status, 0) < 0)// 防止产生僵尸进程
			if(errno != EINTR){
				status = -1;
				break;
			}
	}

	/* mysystem返回前恢复父进程的SIGINT和SIGQUIT的处理方式以及进程的信号集 */
	sigaction(SIGINT, &saveint, NULL);
	sigaction(SIGQUIT, &savequit, NULL);
	sigprocmask(SIG_SETMASK, &savemask, NULL);
	return status;
}

static void sig_int(int signo)
{
	printf("In SIGINT\n");
}

static void sig_chld(int signo)
{
	printf("In SIGQUIT\n");
}

int main(int argc, char const *argv[])
{
	if(signal(SIGINT, sig_int) == SIG_ERR){
		perror("signal SIGINT error");
		return -1;
	}

	if(signal(SIGCHLD, sig_chld) == SIG_ERR){
		perror("signal SIGCHLD error");
		return -1;
	}

	if(mysystem("/bin/ed") < 0){
		perror("system error");
		return -1;
	}
	return 0;
}
/*
执行的结果和10-19system_ed.c是一样的,也就是说目前的system函数的实现是和本文件的实现类似.
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch10_Signals$ ./a.out 
	a
	hello world  
	.
	1,$p
	hello world
	^C
	?
	w temp
	12
	q
	In SIGQUIT

*/