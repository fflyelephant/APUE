#include <signal.h>
#include <errno.h>
#include <stdio.h>

static void print_sigmask(const char *ptr)
{
	sigset_t sigset;
	int 	 errno_save;
	errno_save = errno;
	printf("%s\n", ptr);
	if(sigprocmask(0, NULL, &sigset) < 0){
		perror("sigprocmask error");
		return;
	}else {
		if(sigismember(&sigset, SIGINT))
			printf("\tSIGINT\n");
		if(sigismember(&sigset, SIGQUIT))
			printf("\tSIGQUIT\n");
		if(sigismember(&sigset, SIGUSR1))
			printf("\tSIGUSER1\n");
		if(sigismember(&sigset, SIGALRM))
			printf("\tSIGALRM\n");
	}
	errno = errno_save;//恢复errno值
}

static void sig_int(int signo)
{
	print_sigmask("in sig_int:");
}


int main(int argc, char const *argv[])
{
	sigset_t newmask, oldmask, waitmask;
	print_sigmask("main start:");

	if(signal(SIGINT, sig_int) == SIG_ERR){
		perror("signal SIGINT error");
		return -1;
	}

	sigemptyset(&waitmask);
	sigaddset(&waitmask, SIGUSR1);
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGINT);

	// 设置新的信号屏蔽字(屏蔽SIGINT),保留原始信号屏蔽字到oldmask中
	if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0){
		perror("sigprocmask error");
		return -1;
	}

	// 这里是不希望被SIGINT信号中断的代码临界区

	print_sigmask("in critical region:");

	// sigsuspend函数挂起进程,并且设置挂起阶段的新的信号屏蔽字(只屏蔽SIGUSR1)
	if(sigsuspend(&waitmask) != -1){
		perror("sigsuspend error");
		return -1;
	}
	print_sigmask("return form sigsuspend:");

	// 恢复原先的信号屏蔽字
	if(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0){
		perror("sigprocmask error");
		return -1;
	}

	print_sigmask("over:");
	return 0;
}
/*
	sigsuspend会使用新的信号屏蔽字(waitmask)将进程挂起,直到捕捉到信号,并同信号处理函数一起返回,并恢复调用之前的信号屏蔽字
	sigsuspend函数的意义在于将恢复信号屏蔽字和挂起两个动作作为一个原子操作.

	st@ubuntu:~/git_project/APUE/ch10_Signals$ ./a.out 
	main start:
	in critical region:
		SIGINT
		        --> 程序被sigsuspend挂起
	^Cin sig_int:
		SIGINT
		SIGUSER1
	return form sigsuspend:
		SIGINT  -->sigsuspend 返回后恢复调用前的信号屏蔽字
	over:       -->恢复oldmask

*/