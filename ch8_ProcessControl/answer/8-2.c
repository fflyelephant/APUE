#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

static void fun1(void)
{
	pid_t pid;
	if((pid = vfork()) < 0)
	{
		perror("fork error");
		return;
	}
	/* 父子进程继续向下执行到fun2()函数去 */
}

static void fun2(void)
{
	char buf[1000];
	int i;
	for(i=0; i<sizeof(buf);i++)
		buf[i] = 0;
}


int main(int argc, char const *argv[])
{
	fun1();
	fun2();
	_exit(0);
}
/*
答:首先在fun1函数中调用vfork函数，子进程先执行，指向fun1的栈针，子进程执行fun2函数覆盖了fun1的栈针，栈中前1000字节被赋值为0
   最后子进程_exit()退出，父进程开始执行，执行的结果依赖平台而异

*/