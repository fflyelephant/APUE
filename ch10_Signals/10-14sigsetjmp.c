#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <setjmp.h>
#include <time.h>
static sigjmp_buf 		     jmpbuf;
static volatile sig_atomic_t canjump;

/* 查看当前信号屏蔽字 */
static void print_sigmask(const char* ptr)
{
	sigset_t sigset;
	int 	 errno_save;
	errno_save = errno;
	printf("%s\n", ptr);
	if(sigprocmask(0, NULL, &sigset) < 0){
		perror("sigprocmask error");
		return;
	}else {
		printf("block signal:\n");
		if(sigismember(&sigset, SIGUSR1))
			printf("\tSIGUSER1\n");
		if(sigismember(&sigset, SIGALRM))
			printf("\tSIGALRM\n");
	}
	errno = errno_save;//恢复errno值
}

static void sig_alrm(int signo)
{
	print_sigmask("in sig_alrm");
}

static void sig_usr1(int signo)
{
	time_t starttime;
	if(canjump == 0)
		return;

	print_sigmask("starting sig_usr1:");
	alarm(3);

	starttime = time(NULL);
	for( ; ; )
		if(time(NULL) > starttime + 5)// 保证能让SIGALRM中断此信号处理函数
			break;
	print_sigmask("finishing sig_use1:");
	canjump = 0;
	siglongjmp(jmpbuf, 1);
}

int main(int argc, char const *argv[])
{
	printf("pid:%d\n", getpid());
	if(signal(SIGUSR1, sig_usr1) == SIG_ERR){
		perror("signal sig_usr1 error");
		return -1;
	}

	if(signal(SIGALRM, sig_alrm) == SIG_ERR){
		perror("signal sig_alrm error");
		return -1;
	}

	print_sigmask("starting main:");
	if(sigsetjmp(jmpbuf, 1)){
		print_sigmask("end main:");
		return -1;
	}
	canjump = 1;

	for( ; ; )
		pause();

	return 0;
}
/*

	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch10_Signals$ ./a.out 
	pid:4245
	starting main:
	block signal:
			--> 阻塞等待SIGUSR1信号到来
	starting sig_usr1:
	block signal:
	        SIGUSER1
	        --> SIGUSR1信号处理函数中将本信号阻塞

	in sig_alrm
	block signal:
	        SIGUSER1
	        SIGALRM
			--> 在SIGUSR1处理函数中被SIGALRM信号中断,此时在SIGALRM中被阻塞的信号有SIGALRM和SIGUSR1

	finishing sig_use1:
	block signal:
	        SIGUSER1
	        --> 回到SIGUSR1处理函数中

	end main:
	block signal:
			-->siglongjmp回main函数中,信号屏蔽字恢复为原先的值(无阻塞)

*/