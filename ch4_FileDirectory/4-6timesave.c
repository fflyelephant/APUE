#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	/* 
		将文件截断为0打开，在我的测试系统中，O_TRUNC会修改modify time和change-status time，而last-access time不变
		因为change-status没办法获取和设置，所以这里只保持modify time的值
	*/
	int fd;
	struct stat buf;
	struct timespec oldtime[2] = {0};
	if(argc != 2)
	{
		printf("need a test file\n");
		return -1;
	}

	/* 将modify time保存起来(不跟随符号链接) */
	if(lstat(argv[1], &buf) < 0)
	{
		perror("lstat error");
		return -1;
	}

	//oldtime[0] = 0;		  /* last-access time */  -->(因为我的环境中O_TRUNC不改变它，所以设置为0看看效果)设置最后访问时间0，就是日历时间的起始处(1970年0时0分)
	oldtime[1] = buf.st_mtim; /* modify time */

	if((fd = open(argv[1], O_RDWR | O_TRUNC)) < 0)
	{
		perror("open file error");
		return -1;
	}

	/* 恢复modify time */
	if(futimens(fd, oldtime) < 0)
	{
		perror("futimens error");
		if(fd) close(fd);
		return -1;
	}

	close(fd);
	return 0;
}
/*result:

	stone@ubuntu:~/test_my/github_test/APUE/ch4_file-directory$ ls -l text.file  
	-rw-rw-r-- 1 stone stone 32 Jul 13 15:39 text.file
	stone@ubuntu:~/test_my/github_test/APUE/ch4_file-directory$ ls -lu text.file 
	-rw-rw-r-- 1 stone stone 32 Jul 13 15:39 text.file
	stone@ubuntu:~/test_my/github_test/APUE/ch4_file-directory$ ls -lc text.file 
	-rw-rw-r-- 1 stone stone 32 Jul 13 15:39 text.file
	stone@ubuntu:~/test_my/github_test/APUE/ch4_file-directory$ ./a.out text.file 
	stone@ubuntu:~/test_my/github_test/APUE/ch4_file-directory$ ls -l text.file  -->modify time 没变
	-rw-rw-r-- 1 stone stone 0 Jul 13 15:39 text.file
	stone@ubuntu:~/test_my/github_test/APUE/ch4_file-directory$ ls -lu text.file -->last-access 设置为0(1970)
	-rw-rw-r-- 1 stone stone 0 Jan  1  1970 text.file
	stone@ubuntu:~/test_my/github_test/APUE/ch4_file-directory$ ls -lc text.file -->change-mode time 改变(这个时间没法获取和设置)
	-rw-rw-r-- 1 stone stone 0 Jul 13 15:40 text.file

*/