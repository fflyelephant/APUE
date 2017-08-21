#include <stdio.h>
#include <pthread.h>
#include <error.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char const *argv[])
{
	int 			err;
	struct timespec tout;
	struct tm 		*tmp;
	char 			buf[64];
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(&lock);
	printf("mutex is lock!\n");
	clock_gettime(CLOCK_REALTIME, &tout);
	tmp = localtime(&tout.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);
	printf("current time:%s\n", buf);
	tout.tv_sec += 5;

	pthread_mutex_unlock(&lock);

	// 将在5秒后造成deadlock
	err = pthread_mutex_timedlock(&lock, &tout);
	clock_gettime(CLOCK_REALTIME, &tout);
	tmp = localtime(&tout.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);
	printf("current time:%s\n", buf);
	if(err == 0){
		printf("mutex lock again!\n");
	} else {
		printf("can't lock again! err:%s\n", strerror(err));
	}
	
	return 0;
}
/*
st@ubuntu:~/git_project/APUE/ch11_Threads$ ./a.out 
mutex is lock!
current time:09:44:23 PM
current time:09:44:28 PM
can't lock again! err:Connection timed out
*/