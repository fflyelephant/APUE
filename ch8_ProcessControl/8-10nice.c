#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>

struct timeval end;
unsigned long long count;

static void checktime(const char *name)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	if(tv.tv_sec >= end.tv_sec && tv.tv_usec >= end.tv_usec){
		printf("%s count:%lld\n", name, count);
		exit(0);
	}
}


int main(int argc, char const *argv[])
{
	pid_t pid;
	char *s = NULL;
	int nzero, ret;
	int adj = 0;	

	setbuf(stdout, NULL);// 标准输出无缓冲

#if defined(NZERO)
	nzero = NZERO;	
#elif defined(_SC_NZERO)
	nzero = sysconf(_SC_NZERO);
#else
#error NZERO undefined
#endif

	printf("NZERO=%d\n", nzero);
	if(argc == 2)
		adj = strtol(argv[1], NULL, 10);

	gettimeofday(&end, NULL);
	end.tv_sec += 10;//count++执行最多10秒

	if((pid = fork()) < 0){
		perror("fork error");
		return -1;
	}else if(pid == 0){// 子进程
		s = "child";
		printf("Child nice value:%d, adj:%d\n", nice(0)+nzero, adj);
		errno = 0;
		if((ret = nice(adj)) == -1 && errno != 0)// nice返回-1有两种情况:1<成功返回> 2<失败返回，重置errno>
		{
			perror("set child nice error");
			exit(0);
		}
		printf("Now child nice value:%d\n", ret+nzero);
	}else{// 父进程
		s = "parent";
		printf("Father nice value:%d\n", nice(0)+nzero);
	}

	for(;;){
		count++;
		checktime(s);
	}

	return 0;
}
/*result:

	st@ubuntu:~/git_project/APUE/ch8_ProcessControl$ ./a.out   //当nice值一样时:父子进程占用cpu的比例是(50.24% : 49.76%)
	NZERO=20
	Father nice value:20
	Child nice value:20, adj:0
	Now child nice value:20
	parent count:10434479
	child count:10338932
	st@ubuntu:~/git_project/APUE/ch8_ProcessControl$ ./a.out 20 //当子进程的nice值高19(更低的scheduleing priority)，比例是(98.5% : 1.5%)
	NZERO=20
	Father nice value:20
	Child nice value:20, adj:20
	Now child nice value:39
	parent count:20494970
	child count:311921

*/