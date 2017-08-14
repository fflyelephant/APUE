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
// 这里有一个问题,当SIGALRM信号到来,进入信号处理程序sig_alrm中,这时会设置当前进程的sig_mask,防止信号处理程序
// 在执行过程中被其它信号再次中断,但是信号处理函数在longjmp跳转出去,此时进程的sig_mask是怎么变换的POSIX没有描述,
// 最好是使用10-14中的sigsetjmp和siglongjmp函数来替代