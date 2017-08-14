#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

typedef void Sigfunc(int);//定义一个新类型Sigfunc,表示没有返回值，参数是int的函数

static void sig_handler(int signo)
{
	printf("got a sig_int\n");
}

static Sigfunc *my_signal(int signo, Sigfunc *func)
{
	struct sigaction act, oact;
	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if(signo == SIGALRM){
#ifdef    SA_INTERRUPT
		  act.sa_flags |= SA_INTERRUPT; //只有SIGALRM不重启被中断的系统调用,是为了使用系统调用定时器功能
#endif
	} else {
		  act.sa_flags |= SA_RESTART; //表示如果信号处理函数中断了某个系统调用,系统调用将自动重启
	}

	if(sigaction(signo, &act, &oact) < 0)
		return SIG_ERR;

	return(oact.sa_handler);
}

int main(int argc, char const *argv[])
{
	my_signal(SIGINT, sig_handler);
	sleep(10);
	return 0;
}

/*使用sigaction函数实现signal函数

函数原型:
	int sigaction(int signo, const struct sigaction *restrict act,
							 struct sigaction *restrict oact);
一些结构体:
	struct sigaction {
	1:	void (*sa_handler)(int); //addr of signal handler, or SIG_IGN, or SIG_DFL 
	2:	sigset_t sa_mask;        //additional signals to block 
	3:	int sa_flags;            //signal options, Figure 10.16  
	4:	void (*sa_sigaction)(int, siginfo_t *, void *); //alternate handler 
	};

	1,4都是信号处理函数，只能指定一个，默认使用1作为信号处理函数，但当3是:SA_SIGINFO时使用sa_sigaction
	作为处理函数.
*/