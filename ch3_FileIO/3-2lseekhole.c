#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
	int fd, n;

	if((fd = open("./text.hole",O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0)
	{
		perror("open error");
		exit(EXIT_FAILURE);
	}

	/* 偏移量6 */
	if((n = write(fd, "hello\n", 6)) != 6)
	{
		perror("write first error");
		exit(EXIT_FAILURE);
	}

	/* 偏移量26 */
	if(lseek(fd, 20, SEEK_CUR) == -1)
	{
		perror("lseek error");
		exit(EXIT_FAILURE);
	}

	/* 偏移量32，中间有20个字节的hole */
	if((n = write(fd, "hello\n", 6)) != 6)
	{
		perror("write second error");
		exit(EXIT_FAILURE);
	}

	close(fd);
	return 0;
}
/*result:

	stone@ubuntu:~/test_my/github_test/APUE/ch3_FileIO$ ls -l text.hole
	-rw-rw-r-- 1 stone stone   32 Jul 11 14:16 text.hole   -->文件总大小是32字节
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch3_FileIO$ od -c ./text.hole 
	0000000   h   e   l   l   o  \n  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0   -->其中有20字节的空洞用\0填充了
	0000020  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0   h   e   l   l   o  \n	  -->并且这些空洞不占有实际磁盘块
	0000040
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch3_FileIO$ 

*/