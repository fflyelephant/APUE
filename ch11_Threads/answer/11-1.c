#include <stdio.h>
#include <pthread.h>
#include <error.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

struct foo {
	int a, b, c, d;
};

static void printfoo(const char *str, struct foo *fp)
{
	printf("%s\n", str);
	printf("\tstructure at 0x%lx\n", (unsigned long)fp);
	printf("\tfoo.a = %d\n", fp->a);
	printf("\tfoo.b = %d\n", fp->b);
	printf("\tfoo.c = %d\n", fp->c);
	printf("\tfoo.d = %d\n", fp->d);	
}

static void *thr_fn1(void *arg)
{
	struct foo *foo1 = malloc(sizeof(struct foo));// 在线程1栈上分配的空间
	memset(foo1, 0, sizeof(struct foo));
	foo1->a = 1;
	foo1->b = 2;
	foo1->c = 3;
	foo1->d = 4;
	printfoo("thread 1:\n", foo1);
	pthread_exit((void *)foo1);// 线程1退出后栈空间会被回收被它用
}

static void *thr_fn2(void *arg)
{
	printf("thread 2: ID is %lu\n", (unsigned long)pthread_self());
	pthread_exit((void *)0);
}

int main(int argc, char const *argv[])
{
	int err;
	pthread_t tid1, tid2;
	struct foo *fp;

	err = pthread_create(&tid1, NULL, thr_fn1, NULL);
	if(err != 0){
		perror("pthread_create 1 error");
		return -1;
	}
	err = pthread_join(tid1, (void *)&fp);
	if(err != 0){
		perror("pthread_join 1 error");
		return -1;
	}
	printfoo("main :\n", fp);
	sleep(1);
	free(fp);
	return 0;
}
/*
1:修改为在堆上分配空间就可以避免
st@ubuntu:~/git_project/APUE/ch11_Threads$ ./a.out 
thread 1:

	structure at 0xb6c00468
	foo.a = 1
	foo.b = 2
	foo.c = 3
	foo.d = 4
main :

	structure at 0xb6c00468
	foo.a = 1
	foo.b = 2
	foo.c = 3
	foo.d = 4


*/