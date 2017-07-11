#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define MAX_FD ((int)sysconf(_SC_OPEN_MAX)) /* 一个进程最多能打开的文件数 */
int mydup2(int oldfd, int newfd)
{


}

int main(int argc, char const *argv[])
{
	printf("%d\n", MAX_FD);
	return 0;
}