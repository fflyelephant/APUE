#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <setjmp.h>
static jmp_buf env_alrm;

static void sig_handler(int signo)
{
	longjmp(env_alrm, 1);
}

unsigned int sleep2(unsigned int second)
{
	if(signal(SIGALRM, sig_handler) == SIG_ERR)
	{
		perror("signal error");
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
/*使用函数间跳动解决22,23之前的竞争问题

	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch10_Signals$ ./a.out 
	大约3秒后...
	sleep2:0

 */