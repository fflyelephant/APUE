#include <signal.h>
#include <errno.h>
#include <stdio.h>

volatile sig_atomic_t quitflag;

static void sig_int(int signo)
{
	if(signo == SIGINT)
		printf("interrupt\n");
	else if(signo == SIGQUIT){
		printf("sigquit\n");
		quitflag = 1;
	}
}

int main(int argc, char const *argv[])
{
	sigset_t newmask, oldmask, zeromask;
	if(signal(SIGINT, sig_int) == SIG_ERR){
		perror("signal error");
		return -1;
	}

	if(signal(SIGQUIT, sig_int) == SIG_ERR){
		perror("signal error");
		return -1;
	}

	sigemptyset(&zeromask);
	sigemptyset(&newmask);
	sigemptyset(&oldmask);

	sigaddset(&newmask, SIGQUIT);

	// 当前进程阻塞SIGQUIT信号
	if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0){
		perror("sigprocmask error");
		return -1;
	}

	while(quitflag == 0)
		sigsuspend(&zeromask);// 函数里不阻塞任何信号

	quitflag = 0;

	if(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0){
		perror("sigprocmask error");
		return -1;
	}

	return 0;
}
/*

	st@ubuntu:~/git_project/APUE/ch10_Signals$ ./a.out 
	^CinterruptR
	^Cinterrupt
	^Cinterrupt
	^Cinterrupt
	^Cinterrupt
	^\sigquit

*/