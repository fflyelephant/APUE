#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

extern char **environ;
pthread_mutex_t mutex;
static pthread_once_t init_done = PTHREAD_ONCE_INIT;

static void thread_init(void)
{
	printf("thread_init\n");
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&mutex, &attr);
	pthread_mutexattr_destroy(&attr);
}

/* 可重入版本的getenv */
int myputenv_r(const char *name, const char *value)
{
	int  i = 0;
	char addstr[64] = {0};

	pthread_once(&init_done, thread_init);
	pthread_mutex_lock(&mutex);
	sprintf(addstr, "%s=%s", name, value);
	while(environ[i] != NULL)
		i++;
	strncpy(environ[i-1], addstr, strlen(addstr));
	pthread_mutex_unlock(&mutex);
	return 0;
}

int main(int argc, char const *argv[])
{
	myputenv_r("STONE", "123");
	return 0;
}

/*
1:mygetenv_r是线程安全的函数,而pthread_one指定的thread_init函数仅执行一次
2:mygetenv_r也是可重入的函数

stone@cdWSCMPL07:~/test_my/github_test/APUE/ch12_ThreadsControl$ ./a.out 
thread_init  --> 可以看见,thread_init在两次调用中只执行了一次
result_buf1:/home/stone
result_buf2:/home/stone
*/