#include <stdio.h>
#include <string.h>
#include <errno.h>
#define MAXSTRING 1024
extern char **environ;
static char envbuf[MAXSTRING];

// environ是指针数组,它的每个成员指向一个环境变量,格式为:environ[i] --> "HOME=/home/st"
static char *mygetenv(const char *name)
{
	int i, len;
	len = strlen(name);
	for(i=0; environ[i]!=NULL; i++){
		if((strncmp(name, environ[i], len) == 0) && (environ[i][len] == '=')){
			strncpy(envbuf, &environ[i][len+1], MAXSTRING-1);
			return envbuf;
		}
	}
	return NULL;
}

int main(int argc, char const *argv[])
{	
	printf("HOME:%s\n", mygetenv("HOME"));
	return 0;
}
/*
1:函数mygetenv是不可重入的函数,因为结果都保存在一个静态字符数组里,如果同时有两个线程调用mygetenv,
那么envbuf中的数据是不可预料的.
2:linux下的getenv函数就是不可重入的,它的原型是:
	char *getenv(const char *name);
  这个函数的返回值指向的内存是getenv函数在内部分配的,多线程同时调用会造成内存内容不可预知,实现可重入的getenv函数
  的第一个就是将结果保存在用户维护的内存单元中,这样多个线程自己维护自己的结果,实现可重入:
    char *getenv(const char *name, char *result);

st@ubuntu:~/git_project/APUE/ch12_ThreadsControl$ ./a.out 
HOME:/home/st
*/