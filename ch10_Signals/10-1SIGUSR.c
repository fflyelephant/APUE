#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

static void sig_hanlder(int signo)
{
	if(signo == SIGUSR1)
		printf("Got SIGUSR1 signal!\n");
	else if(signo == SIGUSR2)
		printf("Got SIGUSR2 signal!\n");
	else
		printf("Got a signal:%d\n", signo);
}


int main(int argc, char const *argv[])
{
	if(signal(SIGUSR1, sig_hanlder) == SIG_ERR)
	{
		perror("can't catch SIGUSR1");
		_exit(0);
	}
	
	if(signal(SIGUSR2, sig_hanlder) == SIG_ERR)
	{
		perror("can't catch SIGUSR2");
		_exit(0);
	}

	for( ; ; )
		pause();
	return 0;
}
/*

	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch10_Signals$ ./a.out &
	[1] 12201
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch10_Signals$ kill -USR1 12201
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch10_Signals$ Got SIGUSR1 signal!
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch10_Signals$ kill -USR2 12201
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch10_Signals$ Got SIGUSR2 signal!
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch10_Signals$ kill -9 12201  --> 发送SIGTERM信号给a.out,SIGTERM信号执行默认动作(杀死进程,其实是随后发送了SIGKILL信号给a.out)

*/