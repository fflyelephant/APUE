#include <stdio.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
	char buff[4] = {0};

	while(fgets(buff, 4, stdin) != NULL)/* 行缓冲 */
	{
		if(fputs(buff, stdout) == EOF)
		{
			printf("finish\n");
			return 0;
		}
	}

	if(ferror(stdin))
		printf("input fgets error\n");
	return 0;
}
/*result:

	st@ubuntu:~/git_project/APUE/ch5_StandardIO$ ./a.out 
	wwwwwww
	wwwwwww
	a asf
	a asf
	sdf
	sdf
	www
	www

*/