#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	/* 旧的工作目录 */ 
	if(system("pwd") < 0)
	{
		perror("system error");
		return -1;
	}

	/* 改变工作目录为/tmp */
	if(chdir("/tmp") < 0)
	{
		perror("chdir error");
		return -1;
	}

	/* 新的工作目录 */
	if(system("pwd") < 0)
	{
		perror("system error");
		return -1;
	}

	return 0;
}
/*result:

	st@ubuntu:~/git_project/APUE/ch4_file-directory$ ./a.out 
	/home/st/git_project/APUE/ch4_file-directory  -->旧目录
	/tmp  -->新目录

*/