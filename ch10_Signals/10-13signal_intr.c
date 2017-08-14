#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

typedef void Sigfunc(int);//定义一个新类型Sigfunc,表示没有返回值，参数是int的函数

static void sig_handler(int signo)
{
	printf("got a sig_int\n");
}

static Sigfunc *signal_intr(int signo, Sigfunc *func)
{
	struct sigaction act, oact;
	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if(signo == SIGALRM){
#ifdef    SA_INTERRUPT
		  act.sa_flags |= SA_INTERRUPT; //只有SIGALRM不重启被中断的系统调用,是为了使用系统调用定时器功能
#endif
	} 
	if(sigaction(signo, &act, &oact) < 0)
		return SIG_ERR;

	return(oact.sa_handler);
}

int main(int argc, char const *argv[])
{
	my_signal(SIGINT, signal_intr);
	sleep(10);
	return 0;
}
/*

	signal_intr试图阻止所有被信号中断的系统调用重启

*/