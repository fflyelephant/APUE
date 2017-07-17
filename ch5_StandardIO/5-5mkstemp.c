#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
void make_temp(char *temp)
{
	int fd;
	struct stat st;

	if((fd = mkstemp(temp)) < 0)
	{
		perror("mkstemp error");
		exit(-1);
	}

	printf("temp name:%s\n", temp);
	close(fd);

	if(lstat(temp, &st) < 0)
	{
		if(errno == ENOENT) 
			printf("no this file\n");
		else 
		{
			perror("stat error");
			exit(-1);
		}
	}
	else
	{
		printf("the file exists\n");
		unlink(temp);
	}
}



int main(int argc, char const *argv[])
{
	char good_tmp[] = "/tmp/dirXXXXXX";
	char *bad_tmp   = "/tmp/dirXXXXXX";
	printf("trying to create first temp file ....\n");
	make_temp(good_tmp);

	printf("trying to create second temp file ....\n");
	make_temp(bad_tmp);
	return 0;
}
/*
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch5_StandardIO$ ./a.out 
	trying to create first temp file ....
	temp name:/tmp/dir4gVTrL
	the file exists
	trying to create second temp file ....
	Segmentation fault (core dumped！)

	1:mkstemp的参数是/tmp/dirXXXXXX，XXXXXX这6个大写X是必须的，只能多不能少，函数创建的唯一的文件名将替换这些X
	2:good_tmp是数组成员，在栈上分配，所以mkstemp函数修改XXXXXX时正常修改
	3:bad_tmp中只有指针bad_tmp本身内存在栈上，它指向的内容即"/tmp/dirXXXXXX"是保存在常量区的，是不可以改变的
	  等价于 const char *bad_tmp   = "/tmp/dirXXXXXX"; 所以mkstemp函数试图修改常量区的内容就会core dumped！
*/