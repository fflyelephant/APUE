#include <stdio.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
	int ret;
	while((ret = getc(stdin)) != EOF)
	{
		if(putc(ret, stdout) == EOF)
		{
			printf("finish\n");
			return 0;
		}
	}

	if(ferror(stdin))
		perror("input stdin error");
	return 0;
}
/*result:

	st@ubuntu:~/git_project/APUE/ch5_StandardIO$ ./a.out 
	q    
	q
	www   --> 对getc来说是行缓冲，缓冲区保存了www，遇见回车符getc函数才返回
	www
	q
	q

*/