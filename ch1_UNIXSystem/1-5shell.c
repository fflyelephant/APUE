#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFSIZE 4096
int main(int argc, char const *argv[])
{
	char buff[BUFFSIZE] = {0};
	pid_t pid;
	printf("$ "); /* 模拟命令行提示符 */
	while(fgets(buff, BUFFSIZE, stdin) != NULL){
		if(buff[strlen(buff) - 1] == '\n')
			buff[strlen(buff) -1] = '\0';
		if((pid = fork()) < 0)
		{
			perror("fork error");
			return -1;
		}
		else if(pid == 0)/* 子进程 */
		{
			if(execlp(buff, buff, (char *)0) < 0)/* execlp 启动的程序要求在PATH目录中能找到 */
			{
				perror("execlp error");
				return -1;
			}
			exit(0);
		}

		waitpid(pid, NULL, 0);
		printf("$ ");
	}

	return 0;
}
/*result:

	stone@ubuntu:~/test_my/github_test/APUE/ch1_UNIXSystem$ ./a.out 
	$ ls
	1-1ls.c  1-2read.c  1-3getc.c  1-3ls.c  1-4read.c  1-5shell.c  a.out
	$ pwd
	/home/stone/test_my/github_test/APUE/ch1_UNIXSystem
	$ 

*/