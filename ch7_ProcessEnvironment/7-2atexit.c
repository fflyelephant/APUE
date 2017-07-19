#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

static void exit_fun1(void)
{
	printf("exit_fun1\n");
}

static void exit_fun2(void)
{
	printf("exit_fun2\n");
}

static void exit_fun3(void)
{
	printf("exit_fun3\n");
}

int main(int argc, char const *argv[])
{
	if(atexit(exit_fun1) != 0)
	{
		perror("register exit_fun1 error");
		return -1;
	}	

	if(atexit(exit_fun2) != 0)
	{
		perror("register exit_fun2 error");
		return -1;
	}
	
	if(atexit(exit_fun2) != 0)
	{
		perror("register exit_fun2 error");
		return -1;
	}

	if(atexit(exit_fun3) != 0)
	{
		perror("register exit_fun2 error");
		return -1;
	}
	return 0;
}
/*result:

	st@ubuntu:~/git_project/APUE/7$ ./a.out 
	exit_fun3
	exit_fun2
	exit_fun2
	exit_fun1

*/