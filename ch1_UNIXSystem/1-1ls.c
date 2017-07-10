#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
	DIR * dp;
	struct dirent *dirp;
	if(argc != 2)
	{
		printf("need one dir\n");
		return -1;
	}

	if((dp = opendir(argv[1])) == NULL)/* 打开一个指定目录 */
	{
		perror("opendir error");
		return -1;
	}
	while((dirp = readdir(dp)) != NULL)/* 读出目录下所有目录及文件 */
		printf("%s\n", dirp->d_name);
	closedir(dp);
	return 0;
}
/* result:
	
	stone@ubuntu:~/test_my/github_test/APUE/ch1_UNIXSystem$ ./a.out  ../
	.
	README.md
	ch8_proctl
	.git
	ch1_UNIXSystem
	..

 */