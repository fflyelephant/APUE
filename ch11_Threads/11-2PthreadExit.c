#include <stdio.h>
#include <pthread.h>
#include <error.h>
#include <unistd.h>
#include <stdlib.h>

static void * thr_fn1(void *arg)
{
	printf("pthread1 running.....\n");
	return((void *)1);
}

static void * thr_fn2(void *arg)
{
	printf("pthread2 running.....\n");
	pthread_exit((void *)2);
}

int main(int argc, char const *argv[])
{
	int       err;
	pthread_t tid1, tid2;
	void      *tret;
	// 创建线程1
	err = pthread_create(&tid1, NULL, thr_fn1, NULL);
	if(err != 0){
		perror("pthread_create 1 error");
		return -1;
	}
	// 创建线程2
	err = pthread_create(&tid2, NULL, thr_fn2, NULL);
	if(err != 0){
		perror("pthread_create 2 error");
		return -1;
	}

	// 等待线程1退出
	err = pthread_join(tid1, &tret);
	if(err != 0){
		perror("pthread_join 1 error");
		return -1;
	}
	printf("pthread1 return:%ld\n", (long unsigned int)tret);// 打印线程1 exit code

	// 等待线程2退出
	err = pthread_join(tid2, &tret);
	if(err != 0){
		perror("pthread_join 2 error");
		return -1;
	}
	printf("pthread2 return:%ld\n", (long unsigned int)tret);// 打印线程1 exit code

	return 0;
}
/*
1:pthread_join(pthread_t tid, void **tret) 挂起等待指定线程退出,tret将指向线程退出code(类比进程的waitpid函数)
2:pthread_exit(void *tret) 使线程退出,退出code由tret指向(若想传递更多信息,可以将tret组织为结构体)

st@ubuntu:~/git_project/APUE/ch11_Threads$ ./a.out 
pthread2 running.....
pthread1 running.....
pthread1 return:1  -- pthread_join获得了线程1正常return的值
pthread2 return:2  -- pthread_join获得了线程2pthread_exit退出的值

*/