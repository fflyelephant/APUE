#include <stdio.h>
#include <sys/utsname.h>

/*
	struct utsname {
		char sysname[];
		char nodename[];
		char release[];
		char version[];
		char machine[];
	}

*/

int main(int argc, char const *argv[])
{
	struct utsname name;
	if(uname(&name) < 0)
	{
		perror("uname error");
		return -1;
	}
	printf("name:%s\tnodename:%s\trelease:%s\tversion:%s\tmachine:%s\n", name.sysname, name.nodename, \
            name.release, name.version, name.machine);
	return 0;
}
/*result:

	st@ubuntu:~/git_project/APUE/6$ ./a.out 
	name:Linux	nodename:ubuntu	release:3.11.0-15-generic	version:#25~precise1-Ubuntu SMP Thu Jan 30 17:42:40 UTC 2014	machine:i686

*/