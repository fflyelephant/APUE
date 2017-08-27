#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

static void prepare(void)
{
	int err;
	printf("preparing locks......\n");
	if((err = pthread_mutex_lock(&mutex1)) != 0){
		perror("prepare pthread_mutex_lock mutex1 error");
	}
	if((err = pthread_mutex_lock(&mutex2)) != 0){
		perror("prepare pthread_mutex_lock mutex2 error");
	}
}

static void parent(void)
{
	int err;
	printf("parent unlock1......\n");
	if((err = pthread_mutex_unlock(&mutex1)) != 0){
		perror("parent pthread_mutex_unlock mutex1 error");
	}
	printf("parent unlock2......\n");
	if((err = pthread_mutex_unlock(&mutex2)) != 0){
		perror("parent pthread_mutex_unlock mutex2 error");
	}	
}

static void child(void)
{
	int err;
	printf("child unlock1......\n");
	if((err = pthread_mutex_unlock(&mutex1)) != 0){
		perror("child pthread_mutex_unlock mutex1 error");
	}
	printf("child unlock2......\n");
	if((err = pthread_mutex_unlock(&mutex2)) != 0){
		perror("child pthread_mutex_unlock mutex2 error");
	}	
}

void *thr_fn(void *arg)
{
	printf("thread start......\n");
	pause();
	return((void *)0);
}

int main(int argc, char const *argv[])
{
	int 		err;
	pid_t		pid;
	pthread_t   tid;

	if((err = pthread_atfork(prepare, parent, child)) != 0){
		perror("pthread_atfork error");
		return -1;
	}

	if((err = pthread_create(&tid, NULL, thr_fn, 0)) != 0){
		perror("pthread_create error");
		return -1;
	}
	sleep(2);
	printf("ready to call fork......\n");
	if((pid = fork()) < 0){
		perror("fork error");
		return -1;
	}else if(pid == 0){
		printf("running child process......\n");
	}else{
		printf("running father process......\n");
	}
	return 0;
}
/*
1:函数pthread_atfork是在多线程环境下调用fork时的辅助函数
	int pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void));
参数:
	prepare指向的函数在fork实际调用前调用
	parent指向的函数在fork在父进程中返回前调用
	child指向的函数在fork在子进程中返回前调用

st@ubuntu:~/git_project/APUE/ch12_ThreadsControl$ ./a.out 
thread start......
ready to call fork......
preparing locks......
parent unlock1......
parent unlock2......
running father process......
child unlock1......
child unlock2......
running child process......

*/