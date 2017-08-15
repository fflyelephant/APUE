#include <stdio.h>
#include <signal.h>
#include <error.h>
#include <unistd.h>
#include <stdlib.h>

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

	if(system("/bin/ed") < 0){
		perror("system error");
		return -1;
	}
	return 0;
}
/*

	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch10_Signals$ ./a.out 
	a
	hello world
	.
	1,$p
	hello world
	^C
	?          --> 和书上的例子不同,这里的system是屏蔽了SIGINT信号的,只有子程序ed会捕获SIGINT
	^C
	?
	w temp
	12
	q
	In SIGQUIT 

*/