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

int main(int argc, char const *argv[])
{
	int n;
	char line[1024];
	if(signal(SIGALRM, sig_alrm) != 0){
		perror("signal error");
		return -1;
	}

	if(setjmp(env_alrm) != 0){
		perror("setjmp error");
		return -1;
	}//超时之后longjmp回来继续下一个记时

	alarm(10);//设置定时器,10秒内read没返回sig_alrm将中断read调用
	if((n = read(STDIN_FILENO, line, 1024)) < 0){
		perror("read error");
		return -1;
	}
	alarm(0);//取消定时器
	write(STDIN_FILENO, line, 1024);
	return 0;
}
