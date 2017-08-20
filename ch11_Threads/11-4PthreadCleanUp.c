#include <stdio.h>
#include <pthread.h>
#include <error.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

static void *cleanup(void *arg)
{
	printf("%s\n", (char *)arg);
}

static void *thr_fn1(void *arg)
{
	printf("pthread 1 starting....\n");
	pthread_cleanup_push(cleanup, "thread 1 first handler");
	pthread_cleanup_push(cleanup, "thread 1 second handler");
	printf("pthread 1 cleanup set complete\n");
	if(arg)
		return((void*)1);
	pthread_cleanup_pop(0); // 取消登记的cleanup函数
	pthread_cleanup_pop(0);
	return((void*)0);
}

static void *thr_fn2(void *arg)
{
	printf("pthread 2 starting....\n");
	pthread_cleanup_push(cleanup, "thread 2 first handler");
	pthread_cleanup_push(cleanup, "thread 2 second handler");
	printf("pthread 2cleanup set complete\n");
	if(arg)
		pthread_exit((void*)2);

	pthread_cleanup_pop(0); // 取消登记的cleanup函数
	pthread_cleanup_pop(0);
	return((void*)0);
}

int main(int argc, char const *argv[])
{
	int 	  err;
	pthread_t tid1, tid2;
	void 	  *tret;

	err = pthread_create(&tid1, NULL, thr_fn1, (void *)1);	
	if(err != 0){
		perror("pthread_create thread1 error");
		return -1;
	}
	sleep(1);// 保证线程1先执行
	err = pthread_create(&tid2, NULL, thr_fn2, (void*)2);
	if(err != 0){
		perror("pthread_create thread2 error");
		return -1;
	}

	err = pthread_join(tid1, &tret);
	if(err != 0){
		perror("pthread_join thread1 error");
		return -1;
	}
	printf("thread 1 exit code:%ld\n", (unsigned long)tret);

	err = pthread_join(tid2, &tret);
	if(err != 0){
		perror("pthread_join thread2 error");
		return -1;
	}
	printf("thread 2 exit code:%ld\n", (unsigned long)tret);

	return 0;
}
/*
1:cleanup只在线程异常退出和pthread_exit时执行,return属于正常退出,所以线程1的cleanup函数没有被执行
2:pthread_cleanup_push类似进程中的atexit函数
3:顺便讲一下分离线程,一个线程调用pthread_detach(tid)函数将普通线程变为分离线程,分离线程的意义在于线程的底层存储资源可在线程终止前被回收,
  而普通线程则需pthread_join来回收,用pthread_join来等待一个分离线程结果是未知的.
4:pthread_cleanup_push的典型应用场景是执行锁的释放,当一个线程用了锁访问临界资源后却意外终止了,这是可使用cleanup来进行解锁动作,否则将出现
  之后临界资源无法访问的情况.

st@ubuntu:~/git_project/APUE/ch11_Threads$ ./a.out 
pthread 1 starting....
pthread 1 cleanup set complete
thread 1 exit code:1
pthread 2 starting....
pthread 2cleanup set complete
thread 2 second handler -- cleanup的执行顺序和登记顺序是相反的
thread 2 first handler
thread 2 exit code:2
*/