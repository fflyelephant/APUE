#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

static void sig_handler(int signo)
{
	printf("got a signal\n");
}

unsigned int sleep1(unsigned int second)
{
	if(signal(SIGALRM, sig_handler) == SIG_ERR)
	{
		perror("signal error");
		return second;
	}
	alarm(second);
	pause(); //在18到19行之前有竞争关系，若系统很忙，18行执行后触发了SIGALRM之后才执行pause，则进程将一直挂起(假设无其它信号产生)
	return(alarm(0));
}

int main(int argc, char const *argv[])
{
	unsigned int ret;
	ret = sleep1(3);
	printf("sleep1:%d\n", ret);
	return 0;
}
/*

	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch10_Signals$ ./a.out 
	大约三秒后
	got a signal
	sleep1:0

*/
