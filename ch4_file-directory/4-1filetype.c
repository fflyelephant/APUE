#include <stdio.h>
#include <error.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

const char *getfiletype(const mode_t *mode)
{
	if(NULL == mode)
		return "mode_t has error";
	if(S_ISREG(*mode))
		return "regular";
	else if(S_ISDIR(*mode))
		return "directory";
	else if(S_ISCHR(*mode))
		return "character special";
	else if(S_ISBLK(*mode))
		return "block special";
	else if(S_ISFIFO(*mode))
		return "fifo";
	else if(S_ISLNK(*mode))
		return "link";
	else if(S_ISSOCK(*mode))
		return "socket";
	else
		return "** unknow type **";
}

int main(int argc, char const *argv[])
{
	int i;
	struct stat buf;
	int ret;
	for(i=1; i < argc; i++)
	{
		printf("<file name>:%s ---- ", argv[i]);
		if(lstat(argv[i], &buf) < 0)
		{
			printf("%s lstst error\n", argv[i]);
			return -1;
		}
		printf("<type>:%s\n", getfiletype(&(buf.st_mode)));
	}
	return 0;
}
/*resulte:

	stone@ubuntu:~/test_my/github_test/APUE/ch4_file-directory$ ./a.out /etc/passwd /etc/ /dev/log /dev/tty /dev/sr0 /dev/cdrom  ./ ../
	<file name>:/etc/passwd ---- <type>:regular
	<file name>:/etc/ ---- <type>:directory
	<file name>:/dev/log ---- <type>:link
	<file name>:/dev/tty ---- <type>:character special
	<file name>:/dev/sr0 ---- <type>:block special
	<file name>:/dev/cdrom ---- <type>:link
	<file name>:./ ---- <type>:directory
	<file name>:../ ---- <type>:directory

*/