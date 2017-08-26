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
int mygetenv_r(const char *name, char *result_buf, int result_len)
{
	int i, len, olen;
	pthread_once(&init_done, thread_init);
	len = strlen(name);
	pthread_mutex_lock(&mutex);
	for(i=0; environ[i]!=NULL; i++){
		if((strncmp(name, environ[i], len)==0) && environ[i][len] == '='){
			olen = strlen(&environ[i][len+1]);
			if(olen > result_len){// value值长度大于保存它的内存
				pthread_mutex_unlock(&mutex);
				return(ENOSPC);
			}
			strncpy(result_buf, &environ[i][len+1], result_len-1);
			pthread_mutex_unlock(&mutex);
			return 0;
		}
	}
	pthread_mutex_unlock(&mutex);
	return(ENOENT);
}

int main(int argc, char const *argv[])
{
	char result_buf1[64] = {0};
	char result_buf2[64] = {0};
	mygetenv_r("HOME", result_buf1, sizeof(result_buf1));
	printf("result_buf1:%s\n", result_buf1);
	mygetenv_r("HOME", result_buf2, sizeof(result_buf2));
	printf("result_buf2:%s\n", result_buf2);
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