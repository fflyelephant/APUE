#include <stdio.h>
#include <signal.h>
#include <error.h>
#include <unistd.h>
#include <stdlib.h>

static void myabort(void)
{
	sigset_t mask;
	struct sigaction action;

	/* SIGABRT不应该被忽略,而应该使用默认处理方式 */
	sigaction(SIGABRT, NULL, &action);// 获得SIGABRT信号的处理信息
	if(action.sa_handler == SIG_IGN){
		action.sa_handler = SIG_DFL;
		sigaction(SIGABRT, &action, NULL);
	}

	if(action.sa_handler == SIG_DFL)
		fflush(NULL);
	/* 进程不应该阻塞SIGABRT信号 */
	sigfillset(&mask);
	sigdelset(&mask, SIGABRT);
	sigprocmask(SIG_SETMASK, &mask, NULL);
	kill(getpid(), SIGABRT);// 唤起SIGABRT信号,程序到这里应该被SIGABRT的默认动作终止了,如果在myabort外部有针对
	 						// SIGABRT信号的处理函数且正常返回会继续向下走第二此唤起SIGABRT信号
	
	//到这里表示SIGABRT被捕获且返回,下面会再次唤起SIGABRT信号
	fflush(NULL);
	action.sa_handler = SIG_DFL;
	sigaction(SIGABRT, &action, NULL);
	sigprocmask(SIG_SETMASK, &mask, NULL);
	kill(getpid(), SIGABRT);//终止进程

	exit(1);// 程序不应该走到这里
}


int main(int argc, char const *argv[])
{
	printf("main\n");
	myabort();
	printf("mian over\n");
	return 0;
}

/*
abort的man描述
DESCRIPTION
       The  abort()  first  unblocks  the  SIGABRT  signal, and then raises that signal for the calling process (as though raise(3) was
       called).  This results in the abnormal termination of the process unless the SIGABRT signal is caught  and  the  signal  handler
       does not return (see longjmp(3)).
       If the abort() function causes process termination, all open streams are closed and flushed.
       If  the  SIGABRT  signal is ignored, or caught by a handler that returns, the abort() function will still terminate the process.
       It does this by restoring the default disposition for SIGABRT and then raising the signal for a second time.
	
	译:abort()首先解阻塞SIGABRT信号,然后在调用进程中唤起SIGABRT信号,结果使进程异常终止,除非SIGABRT信号被处理函数
	   捕获且未返回(比如处理函数返回前调用了longjmp).

	   abort()函数让进程终止前,会冲洗和关闭所有的流.

	   当SIGABRT信号被忽略或者其信号处理函数正常返回,abort()仍然会终止进程,这是通过恢复SIGABRT的处理方式并且第二次
	   唤起SIGABRT信号来实现的.
*/

/*

	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch10_Signals$ ./a.out 
	main
	Aborted (core dumped)
	
*/