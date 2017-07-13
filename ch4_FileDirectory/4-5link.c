#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char const *argv[])
{
#if 1
	if(linkat(AT_FDCWD, "./softlink", AT_FDCWD, "./mylink", 0) < 0)
#else
	if(linkat(AT_FDCWD, "./softlink", AT_FDCWD, "./mylink", AT_SYMLINK_FOLLOW) < 0)
#endif
	//if(link("./a.out", "./softlink") < 0)
	{
		perror("linkat error");
		return -1;
	}

	sleep(10);

	if(unlinkat(AT_FDCWD, "./mylink" ,0) < 0)
	{
		perror("unlinkat error");
		return -1;
	}

	return 0;
}
/*result:

	AT_SYMLINK_FOLLOW:是linkat的flag，如果设置了创建的新link会指向oldlink指向的问价，否则执行oldlink自己
	AT_REMOVEDIR:是unlinkat的flag，如果unlink的是一个目录，会删除它.
	
	stone@ubuntu:~/test_my/github_test/APUE/ch4_file-directory$ ls -l *link* 4-ftruncate.c 
	-rw-rw-r-- 1 stone stone  776 Jul 13 10:45 4-5link.c
	-rw-rw-r-- 1 stone stone 1077 Jul 13 10:30 4-ftruncate.c
	lrwxrwxrwx 1 stone stone   13 Jul 13 10:43 softlink -> 4-ftruncate.c  --> 预先创建的软连接
	stone@ubuntu:~/test_my/github_test/APUE/ch4_file-directory$ ./a.out &
	[1] 8905
	stone@ubuntu:~/test_my/github_test/APUE/ch4_file-directory$ ls -l *link* 4-ftruncate.c 
	-rw-rw-r-- 1 stone stone  776 Jul 13 10:45 4-5link.c
	-rw-rw-r-- 2 stone stone 1077 Jul 13 10:30 4-ftruncate.c
	
	-rw-rw-r-- 2 stone stone 1077 Jul 13 10:30 mylink  -->设置了AT_SYMLINK_FOLLOW，所以新的连接指向软连接指向的文件4-ftruncate.c
或者:lrwxrwxrwx 2 stone stone   13 Jul 13 10:43 mylink -> 4-ftruncate.c -->未设置AT_SYMLINK_FOLLOW

	lrwxrwxrwx 1 stone stone   13 Jul 13 10:43 softlink -> 4-ftruncate.c
	stone@ubuntu:~/test_my/github_test/APUE/ch4_file-directory$ ls -l *link* 4-ftruncate.c 
	-rw-rw-r-- 1 stone stone  776 Jul 13 10:45 4-5link.c
	-rw-rw-r-- 1 stone stone 1077 Jul 13 10:30 4-ftruncate.c
	lrwxrwxrwx 1 stone stone   13 Jul 13 10:43 softlink -> 4-ftruncate.c
	-->mylink被删除了
*/