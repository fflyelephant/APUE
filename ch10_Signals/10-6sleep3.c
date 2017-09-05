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


unsigned int sleep3(unsigned int second)
{
	if(signal(SIGALRM, sig_alrm) == SIG_ERR){
		perror("signal alrm error");
		return second;
	}

	if(setjmp(env_alrm) == 0){//成功设置返回0，从longjmp跳回返回非零
		alarm(second);
		printf("123\n");
		pause(); 
	}

	return(alarm(0));
}

int main(int argc, char const *argv[])
{
	#if 0
	if(signal(SIGINT, sig_int) == SIG_ERR){
		perror("signal int error");
		return -1;
	}
	#endif
	unsigned int ret;
	ret = sleep3(3);
	printf("sleep3:%d\n", ret);
	return 0;
}
/*

	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch10_Signals$ ./a.out 
	^C --> SIGINT信号同样会唤醒pause(),pause()将阻塞直到sig_int函数执行完之后才返回,因为sig_int函数会执行超过
		   3秒,所以在sig_int执行期间SIGALRM信号产生,导致longjmp执行,sleep3()会直接返回(此时sig_int可能还在执行中)
	sig_int start --> 开始执行sig_int处理函数
	sleep2:0

	使用标准sleep函数会等待sig_int执行完毕后sleep才返回,返回值表示sleep在sig_int函数执行前已经终止,因为标准sleep
	函数使用sigsuspend函数阻塞,此函数会等待信号的处理函数处理完之后才返回.
标准版:
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch10_Signals$ ./a.out 
	^C 					--> 执行2秒之后才按下SIGINT
	sig_int start		
	sig_int finished	--> sig_int完整执行完(>3秒)
	sleep3:1			--> 标准sleep返回,表示只sleep了2秒,剩1秒未sleep

注:pause()和sigsuspend()都是能被信号唤醒并等待信号处理函数执行完之后再返回,区别就是sigsuspend()函数可以指定信号
   屏蔽集,这样可以选择被指定的信号唤醒
 */