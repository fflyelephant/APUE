#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH) /* 权限值0666 */

int main(int argc, char const *argv[])
{
	umask(0); /* umask=0000 */
	if(creat("./foo", RWRWRW) < 0)
	{
		perror("creat foo error");
		return -1;
	}

	umask(S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH); /* umask=0066 */
	if(creat("./bar", RWRWRW) < 0)
	{
		perror("creat bar error");
		return -1;
	}

	return 0;
}
/*result:

	stone@ubuntu:~/test_my/github_test/APUE/ch4_file-directory$ ls -l bar foo 
	-rw------- 1 stone stone 0 Jul 12 17:43 bar
	-rw-rw-rw- 1 stone stone 0 Jul 12 17:43 foo

	解析:
		bar:权限是0600，即: 0666&~0066 = 0600
		foo:权限是0666，即: 0666&~0000 = 0666
		所以 权限结果 = mode&~umask
*/