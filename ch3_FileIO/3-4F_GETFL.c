#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
	int val;
	if(argc != 2)
	{
		printf("need a file name\n");
		return -1;
	}
	/* F_GETFL获取访问属性 */
	if((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0)
	{
		perror("fcntl error");
		return -1;
	}

	/* 基础访问属性 */
	switch(val & O_ACCMODE){
		case O_RDONLY:
			printf("read only");
			break;
		case O_WRONLY:
			printf("write only");
			break;
		case O_RDWR:
			printf("read or write");
			break;
		default:
			printf("unknow access mode");
	}

	/* 扩展访问属性 */
	if(val & O_APPEND)
		printf(", append");
	if(val & O_NONBLOCK)
		printf("nonblock");
	if(val & O_SYNC)
		printf("sync");
#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)
	if(val & O_FSYNC)
		printf("fsync");
#endif	
	putchar('\n');
	return 0;
}
/*result:

	stone@ubuntu:~/test_my/github_test/APUE/ch3_FileIO$ ./a.out 0 < /dev/tty  -->标准输入文件执行只读的/dev/tty
	read only
	stone@ubuntu:~/test_my/github_test/APUE/ch3_FileIO$ ./a.out 1 > temp -->标准输出是只写的
	stone@ubuntu:~/test_my/github_test/APUE/ch3_FileIO$ cat temp 
	write only
	stone@ubuntu:~/test_my/github_test/APUE/ch3_FileIO$ ./a.out 2 2>>temp  -->把标准错误重定向到temp
	write only, append
	stone@ubuntu:~/test_my/github_test/APUE/ch3_FileIO$ ./a.out 5 5<>temp  -->描述符5的访问属性同temp文件一样
	read or write

 */