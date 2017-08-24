#include <stdio.h>
#include <pthread.h>

int global = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;

void process_msg(void)
{
	for( ; ; ){
		pthread_mutex_lock(&mutex);
		while(global == 0)
			pthread_cond_wait(&cond, &mutex);
		// 条件满足后
		// 执行满足条件之后的代码
		pthread_mutex_unlock(&mutex);
	}
}

void enqueue_msg(void)
{
	pthread_mutex_lock(&mutex);
	// 改变glbal的值
	pthread_mutex_unlock(&mutex);
	// 唤起一个阻塞进程
	pthread_cond_signal(&cond);
}

/*
1:process_msg函数执行在某一个线程中,此时的条件为global不为0,一旦检测到就执行条件满足后的代码
2:enqueue_msg函数改变条件,之后使用signal通知某一个阻塞的进程条件已满足
*/