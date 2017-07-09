#include <stdio.h>		/* for convenience */
#include <unistd.h>
#include <sys/types.h>
 
int main(int argc, char const *argv[])
{
	extern char **environ;
	int i;
	char **ptr;
	printf("echoall pid:%d\n", getpid());
	/* 输出所有的命令参数 */ 
	for(i=0; i<argc; i++)
		printf("argv[%d]:%s\n", i,argv[i]);

	/* 输出所有的环境参数 */
	for(ptr=environ; *ptr != NULL; *ptr++)
		printf("%s\n", *ptr);

	return 0;
}