#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>

  int 	 	quitflags;
sigset_t 	mask;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;

static void *thr_fn(void *arg)
{
	int err, signo;
	for( ; ; ){
		err = sigwait(&mask, &signo);// sigwait在mask的信号屏蔽字条件下阻塞等待信号到来
		if(err != 0){
			perror("sigwait error");
			return (void *)-1;
		}
		switch(signo){
			case SIGINT:
				printf("\ninterrupt\n");
				break;
			case SIGQUIT:
				printf("\nsigquit\n");
				pthread_mutex_lock(&mutex);
				quitflags = 1;
				pthread_mutex_unlock(&mutex);
				pthread_cond_signal(&cond);
				return((void*)0);
			default:
				printf("unexpected signal %d\n", signo);
				exit(1);
		}
	}
	return((void *)1);
}

int main(int argc, char const *argv[])
{
	int 		err;
	sigset_t	old_mask;
	pthread_t 	tid;

	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGQUIT);
	if((err = pthread_sigmask(SIG_BLOCK, &mask, &old_mask)) != 0){
		perror("pthread_sigmask error");
		return -1;
	}

	if((err = pthread_create(&tid, NULL, thr_fn, 0)) != 0){
		perror("pthread_create error");
		return -1;
	}

	pthread_mutex_lock(&mutex);
	while(quitflags==0)
		pthread_cond_wait(&cond, &mutex);
	pthread_mutex_unlock(&mutex);

	quitflags = 0;
	if(sigprocmask(SIG_SETMASK, &old_mask, NULL) != 0){
		perror("sigprocmask error");
		return -1;
	}
	return 0;
}
/*
1:程序描述了在多线程中使用信号处理的典型例子,思路就是:
  1.1 先在主进程中屏蔽关注的信号
  2.2 利用线程继承进程的屏蔽集特点创建线程,在线程中调用sigwait函数等待被阻塞的信号
2:API描述(在多线程的程序中尽量使用多线程信号相关API)
  2.1: 
  		int pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset);
  		函数类似进程中的sigprocmask,指定对set描述集的动作,how的取值有:SIG_BLOCK, SIG_SETMASK, SIG_UNBLOCK

  2.2:
  		int sigwait(const sigset_t *set, int *signop);
		调用此函数前应该阻塞线程等待的那些信号,函数会执行以下步骤:
		A:原子的取消信号集的阻塞状态,直到有新的信号被递送
		B:返回之前将恢复信号集
	注:在线程中使用signal或者sigaction会使得除本线程外的所有其他线程也具有相同的信号处理方式,所以在多线程中最好使用sigwait函数来保证
	某信号的处理方式仅仅在本线程中有效

  2.3:  
  		int pthread_kill(pthread_t thread, int signo);
		替代kill函数,kill函数是对进程发送信号,在多线程环境下导致所有线程都收到该信号

st@ubuntu:~/git_project/APUE/ch12_ThreadsControl$ ./a.out 
^C
interrupt
^C
interrupt
^C
interrupt
^C
interrupt
^\
sigquit
st@ubuntu:~/git_project/APUE/ch12_ThreadsControl$

*/