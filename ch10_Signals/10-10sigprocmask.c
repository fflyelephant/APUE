#include <signal.h>
#include <errno.h>
#include <stdio.h>

static void print_sigmask(void)
{
	sigset_t sigset;
	int 	 errno_save;
	errno_save = errno;

	if(sigprocmask(0, NULL, &sigset) < 0){
		perror("sigprocmask error");
		return;
	}else {
		printf("some signal are blocked!\n");
		if(sigismember(&sigset, SIGINT))
			printf("SIGINT\n");
		if(sigismember(&sigset, SIGQUIT))
			printf("SIGQUIT\n");
		if(sigismember(&sigset, SIGUSR1))
			printf("SIGUSER1\n");
		if(sigismember(&sigset, SIGALRM))
			printf("SIGALRM\n");
	}
	printf("over!\n");
	errno = errno_save;//恢复errno值
}

int main(int argc, char const *argv[])
{
	print_sigmask();
	return 0;
}
/*
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch10_Signals$ ./a.out 
	some signal are blocked!
	over!
	-->表示目前没有信号被屏蔽
*/