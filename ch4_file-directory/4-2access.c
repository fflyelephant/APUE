#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{
	int fd;
	char buf[64] = {0};
	if(argc != 2)
	{
		printf("need a file\n");
		return -1;
	}
	/* 检验有没有读权限 */
	if(access("./123.txt", R_OK) < 0)
	{
		perror("access error");
		return -1;
	}

	/* 按只读打开文件 */
	if((fd = open("./123.txt", O_RDONLY)) < 0)
	{
		perror("open error");
		return -1;
	}

	read(fd, buf, sizeof(buf));
	write(STDOUT_FILENO, buf, sizeof(buf));
	close(fd);
	return 0;
}
/*result:

	stone@ubuntu:~/test_my/github_test/APUE/ch4_file-directory$ ls -l 123.txt 
	-rw-rw-r-- 1 stone stone 10 Jul 12 17:13 123.txt
	stone@ubuntu:~/test_my/github_test/APUE/ch4_file-directory$ ./a.out 123.txt 
	123
	qwe
	stone@ubuntu:~/test_my/github_test/APUE/ch4_file-directory$ chmod 0064 ./123.txt 
	stone@ubuntu:~/test_my/github_test/APUE/ch4_file-directory$ ls -l 123.txt 
	----rw-r-- 1 stone stone 10 Jul 12 17:13 123.txt
	stone@ubuntu:~/test_my/github_test/APUE/ch4_file-directory$ ./a.out 123.txt 
	access error: Permission denied
	stone@ubuntu:~/test_my/github_test/APUE/ch4_file-directory$ chmod 0664 ./123.txt 
	stone@ubuntu:~/test_my/github_test/APUE/ch4_file-directory$ ls -l 123.txt 
	-rw-rw-r-- 1 stone stone 10 Jul 12 17:13 123.txt
	stone@ubuntu:~/test_my/github_test/APUE/ch4_file-directory$ ./a.out 123.txt 
	123
	qwe

*/