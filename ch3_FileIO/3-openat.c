#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int main(int agrc, char *argv[])
{
	int dirfd;
	int fd;
	/* 打开一个目录 */
	if((dirfd = open("./test", O_RDONLY | O_DIRECTORY)) < 0)
	{
		perror("open dirfd error");
		exit(EXIT_FAILURE);
	}
#if 0 
	/* 在dirfd所表示的目录下打开文件text.txt */
	if((fd = openat(dirfd, "./text.txt", O_RDWR | O_CREAT, 0666)) < 0)
	{
		perror("openat error");
		exit(EXIT_FAILURE);
	}

	/* pathname是绝对路径，忽略dirfd，直接创建/home/stone/test_my/text.txt文件 */
	if((fd = openat(dirfd, "/home/stone/test_my/text.txt", O_RDWR | O_CREAT, 0666)) < 0)
	{
		perror("openat error");
		exit(EXIT_FAILURE);
	}
#endif
	/* 无论pathname是相对路径还是绝对路径<必须是指向一个文件>，只要dirfd是AT_FDCWD，openat和open等价 */
	if((fd = openat(AT_FDCWD, "./text.txt", O_RDWR | O_CREAT, 0666)) < 0)
	{
		perror("openat error");
		exit(EXIT_FAILURE);
	}

	write(fd, "hello text.txt\n", 15);
	close(fd);
	close(dirfd);
	return 0;
}

