#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	fprintf(stderr, "EACCES:%s\n", strerror(EACCES));
	errno = ENOENT;/* errno 属于全局变量*/
	perror(argv[0]);
	return 0;
}
/*result:

	stone@ubuntu:~/test_my/github_test/APUE/ch1_UNIXSystem$ ./a.out 
	EACCES:Permission denied
	./a.out: No such file or directory

*/