#include <stdio.h>
#include <pthread.h>
#include <error.h>
#include <unistd.h>
#include <stdlib.h>
pthread_t ntid;// 保存新线程的id

static void printids(const char *str)
{
	pid_t     pid;
	pthread_t tid;
	pid = getpid();
	tid = pthread_self();
	printf("%s\n\tpid:%lu, tid:%lu(0x%lx)\n", str, (long unsigned int)pid, \
										(long unsigned int)tid, (long unsigned int)tid);
}

static void * thr_fn(void *arg)
{
	printids("new pthread:");
	return((void *)0);
}

int main(int argc, char const *argv[])
{
	int err;
	err = pthread_create(&ntid, NULL, thr_fn, NULL);
	if(err != 0){
		perror("pthread_create error");
		return -1;
	}
	printids("main thread:");
	sleep(1);// 主进程不等待的话直接退出,子线程可能没有机会得到执行
	return 0;
}
/*
1:新线程的ID会保存在全局变量ntid中,但是在新线程中还是使用pthread_self来获取ID,这样做的原因是防止在执行printid函数时,pthread_create函数还未返回,
  这时的ntid还是进程静态区为初始化的变量.
st@ubuntu:~/git_project/APUE/ch11_Threads$ gcc 11-1PrintPthreadId.c -lpthread
st@ubuntu:~/git_project/APUE/ch11_Threads$ ./a.out 
main thread:
	pid:10037, tid:3075839680(0xb75596c0)
new pthread:
	pid:10037, tid:3075836736(0xb7558b40)

线程的pid和进程是一样的,线程ID是不同的,因为pthread_t是一个结构体,只能打印结构体的值来表示线程ID
*/