#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

#define LEN 1024
extern char **environ;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_key_t key;
static pthread_once_t init_done = PTHREAD_ONCE_INIT;

static void thread_init(void)
{
	printf("thread_init\n");
	pthread_key_create(&key, free);
}

/* 不可重入版本的getenv(因为分配私有数据的时候使用了malloc这个不可重入的函数) */
char *mygetenv(const char *name)
{
	int i, len;
	char *result_buf;
	pthread_once(&init_done, thread_init);
	pthread_mutex_lock(&mutex);
	result_buf = (char *)pthread_getspecific(key);// 获得key值关联的私有存储区首地址
	if(result_buf == NULL){
		result_buf = malloc(LEN);
		if(result_buf  == NULL){
			pthread_mutex_unlock(&mutex);
			return NULL;
		}
		pthread_setspecific(key, result_buf);
	}
	len = strlen(name);
	for(i=0; environ[i]!=NULL; i++){
		if((strncmp(name, environ[i], len)==0) && environ[i][len] == '='){
			strcpy(result_buf, &environ[i][len+1]);
			pthread_mutex_unlock(&mutex);
			return result_buf;
		}
	}
	pthread_mutex_unlock(&mutex);
	return NULL;
}
int main(int argc, char const *argv[])
{
	printf("first:%s\n", mygetenv("HOME"));
	printf("second:%s\n", mygetenv("HOME"));
	return 0;
}

/*
1:使用线程私有数据机制分配空间给查找结果,因为线程之间是共享进程的数据,而这个机制提供了线程之间数据独立的实现方式,典型的例子就是线程中的errno
  它在每个线程中都是互相独立的
2:
  2.1:创建私有数据标识key值,指定私有数据的析构函数
  		int pthread_key_create(pthread_key_t *key, void (*destructor)(void *));
  			     key   --> 标识了一个私有数据,这个键值可以被同一进程中的所有线程访问,但是每个线程又将键值与自己特定的数据地址进行关联
  			destructor --> 私有数据的析构函数,参数(void *)默认是私有数据的起始地址,析构函数只在线程正常退出(return或者调用pthread_exit)的时候调用
  2.2:获得和设置私有数据key值对应的私有数据首地址
  		void *pthread_getspecific(pthread_key_t key);
  		int   pthread_setspecific(pthread_key_t key, const void *value);
  2.3:取消键值和私有数据地址的联系
  		int   pthread_key_delete(pthread_key_t key);// 函数并不会释放与键关联的数据内存(慎用)

st@ubuntu:~/git_project/APUE/ch12_ThreadsControl$ ./a.out 
thread_init
first:/home/st
second:/home/st

*/