#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
	int fd[2], pid;
	char *p = "hello,Mrs tree\n";
	char str[64] = {0};
	if(pipe(fd) < 0){
		perror("pipe error");
		return -1;
	}

	if((pid = fork()) < 0){
		perror("fork error");
		return -1;
	} else if(pid == 0){
		close(fd[0]);
		if(write(fd[1], p, strlen(p)) != strlen(p)){
			perror("write error");
		}
	} else {
		close(fd[1]);
		if(read(fd[0], str, sizeof(str)) < 0){
			perror("read error");
		}
		waitpid(pid, NULL, 0);
		printf("str:%s", str);
	}
	return 0;
}
/*
1:子进程向父进程传递字符串,pipe只能使用在具有亲缘关系的进程间

	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch15_IPC$ ./a.out 
	str:hello,Mrs tree
*/