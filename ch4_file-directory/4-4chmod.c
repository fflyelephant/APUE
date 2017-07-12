#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

#define RWRWRW (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) /* 权限制0666 */
int main(int argc, char const *argv[])
{
	struct stat buf;

	/* 设置foo文件的相对访问属性 */
	if(stat("./foo", &buf) < 0)
	{
		perror("stat foo error");
		return -1;
	}
	/* 关闭了组执行位，开启了设置组ID位 */
	if(chmod("./foo", (buf.st_mode & ~S_IXGRP) | S_ISGID) < 0)
	{
		perror("foo chmod error");
		return -1;
	}

	/* 设置bar的绝对访问属性 */
	if(chmod("./bar", RWRWRW) < 0)
	{
		perror("bar chmod error");
		return -1;
	}

	return 0;
}
/*result:

	st@ubuntu:~/git_project/APUE/ch4_file-directory$ ls -l bar foo 
	-rw------- 1 st st 0 Jul 12 21:19 bar
	-rw-rw-rw- 1 st st 0 Jul 12 21:19 foo
	st@ubuntu:~/git_project/APUE/ch4_file-directory$ ./a.out 
	st@ubuntu:~/git_project/APUE/ch4_file-directory$ ls -l bar foo 
	-rw-rw-rw- 1 st st 0 Jul 12 21:19 bar
	-rw-rwSrw- 1 st st 0 Jul 12 21:19 foo

*/