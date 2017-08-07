#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <setjmp.h>
static jmp_buf env_alrm;

static void sig_alrm(int signo)
{
	longjmp(env_alrm, 1);
}

/* 处理函数的执行时间将超过3秒钟 */
static void sig_int(int signo)
{
	int i, j;
	volatile int k;
	printf("\nsig_int start\n");
	for(i=0; i<300000; i++)
		for(j=0; j<4000; j++)
			k += i*j;
	printf("sig_int finished\n");
}


unsigned int sleep2(unsigned int second)
{
	if(signal(SIGALRM, sig_alrm) == SIG_ERR){
		perror("signal alrm error");
		return second;
	}

	if(signal(SIGINT, sig_int) == SIG_ERR){
		perror("signal int error");
		return second;
	}

	if(setjmp(env_alrm) == 0){//成功设置返回0，从longjmp跳回返回非零
		alarm(second);
		pause(); 
	}

	return(alarm(0));
}

int main(int argc, char const *argv[])
{
	unsigned int ret;
	ret = sleep2(3);
	printf("sleep2:%d\n", ret);
	return 0;
}
/*

	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch10_Signals$ ./a.out 
	^C --> SIGINT
	sig_int start --> 开始执行sig_int处理函数
	sleep2:0  -->在执行sig_int函数期间被longjmp函数强行中断返回，所以longjmp函数会中断其它信号处理函数的执行

 */