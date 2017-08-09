#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

static void sig_quit(int signo)
{
	printf("got SIGQUIT\n");
	if(signal(SIGQUIT, SIG_DFL) == SIG_ERR){
		perror("signal in sig_quit error");
		return;
	}
}

int main(int argc, char const *argv[])
{
	sigset_t  newmask, oldmask, pendmask;
	if(signal(SIGQUIT, sig_quit) == SIG_ERR){
		perror("signal error");
		return -1;
	}

	sigemptyset(&newmask);//先清空信号集
	sigaddset(&newmask, SIGQUIT);//再将SIGQUIT信号设置如信号集中
	if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0){//设置信号集阻塞的同时保存当前的信号集到oldmask中
		perror("sigprocmask error");
		return -1;
	}

	sleep(5);//SIGQUIT将在这里被block

	if(sigpending(&pendmask) < 0){//获得当前被pending的信号集
		perror("sigpending error");
		return -1;
	}
	if(sigismember(&pendmask, SIGQUIT))
		printf("SIGQUIT signal is pending\n");

	//恢复原先的信号集
	if(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0){
		perror("sigprocmask error");
		return -1;
	}
	printf("SIGQUIT unblock\n");
	return 0;
}

/*
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch10_Signals$ ./a.out 
	^\^\^\
	SIGQUIT signal is pending  
	got SIGQUIT -->这里的三个SIGQUIT信号解除block之后只有一个信号被发送到了进程
	SIGQUIT unblock
*/