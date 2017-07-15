#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	char pathname[64] = {0};
	
	/* 旧的工作目录 */ 
	if (getcwd(pathname, 64) == NULL)
	{
		perror("getcwd error");
		return -1;
	}
	printf("before chdir path:%s\n", pathname);


	/* 改变工作目录为/tmp */
	if(chdir("/tmp") < 0)
	{
		perror("chdir error");
		return -1;
	}

	/* 新的工作目录 */
	if (getcwd(pathname, 64) == NULL)
	{
		perror("getcwd error");	
		return -1;
	}
	printf("after chdir path:%s\n", pathname);

	return 0;
}
/*result:

	st@ubuntu:~/git_project/APUE/ch4_file-directory$ ./a.out 
	before chdir path:/home/st/git_project/APUE/ch4_FileDirectory
	after chdir path:/tmp

*/