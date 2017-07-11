#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define OPEN_MAX ((int)sysconf(_SC_OPEN_MAX)) /* 一个进程最多能打开的文件数，也是文件描述符的最大值 */
int mydup2(int oldfd, int newfd)
{
	int i, j, fd[OPEN_MAX];
	/* 首先检验文件描述符的范围是否正确 */
	if (oldfd < 0 || oldfd > (OPEN_MAX-1))
	{
		perror("oldfd error");
		return -1;
	}
	if(newfd < 0 || newfd > (OPEN_MAX - 1))
	{
		perror("newfd error");
		return -1;
	}
	/* 相等时返回newfd */
	if(oldfd == newfd)
		return newfd;
	/* 先关闭newfd，好让dup时能有机会选择newfd作为返回值 */
	close(newfd);
	for(i=0; i<OPEN_MAX; i++)
	{
		if((fd[i] = dup(oldfd)) < 0)
		{
			perror("dup error");
			return -1;
		}

		/* 关掉之前dup出来的文件描述符 */
		if(fd[i] == newfd)
		{
			for(j=0; j<i; j++)
				close(fd[j]);
			return newfd;
		}
	}
	return -1;
}

int main(int argc, char const *argv[])
{
	int retfd;
	if((retfd = mydup2(2, 5)) < 0)
	{
		perror("mydup2 error");
		return -1;
	}
	printf("dup result:%d\n", retfd);
	return 0;
}
/* result:

	st@ubuntu:~/git_project/APUE/ch3_FileIO/answer$ ./a.out 
	dup result:5  -->返回值是newfd参数说明mydup2是正确执行了的

*/