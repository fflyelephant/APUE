#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

static void sig_alrm(int signo)
{
	;// 仅仅是为了唤起sigsuspend函数
}

static void sig_int(int signo)
{
	;// 测试使用SIGINT去中断sigsuspend函数
}

static unsigned int mysleep(unsigned int seconds)
{
	struct sigaction newact, oldact;
	sigset_t 		 newmask, oldmask, susmask;
	unsigned int 	 unslept;

	signal(SIGINT, sig_int);
	// 设置SIGALRM处理函数,保存旧处理方式
	newact.sa_handler = sig_alrm;
	sigemptyset(&newact.sa_mask);// sa_mask也是进程mask,但是它的作用时间是在调用该信号捕捉函数之前,捕捉函数返回之后进程mask恢复原值
	newact.sa_flags = 0;
	sigaction(SIGALRM, &newact, &oldact);	

	// 设置进程mask,阻塞SIGALRM信号,保存当前进程mask
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGALRM);
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);
	// 程序至此,进程虽然有SIGALRM信号的处理函数,但是进程mask却阻塞了此信号

	alarm(seconds);
	susmask = oldmask;

	// 确保SIGALRM不被阻塞
	sigdelset(&susmask, SIGALRM);

	// 进程挂起直到被某个信号唤起
	sigsuspend(&susmask);
	// 某个信号到来唤起进程,最好是SIGALRM,如果是就表示计时时间到

	unslept = alarm(0);// 取消定时器
	// unslept = 0 表示sigsuspend函数被SIGALRM唤起,计时跑完
	// unslept > 0 表示sigsuspend函数被其它信号唤起,计时未跑完,返回剩余时间

	// 恢复之前的SIGALRM处理函数和进程mask
	sigaction(SIGALRM, &oldact, NULL);
	sigprocmask(SIG_SETMASK, &oldmask, NULL);
	printf("unslept:%d\n", unslept);
	return unslept;
}

int main(int argc, char const *argv[])
{	
	mysleep(5);
	return 0;
}
/*

st@ubuntu:~/git_project/APUE/ch10_Signals$ ./a.out 
^Cunslept:2  -->SIGINT中断了sigsuspend函数,alarm还剩下2秒没有跑完


*/