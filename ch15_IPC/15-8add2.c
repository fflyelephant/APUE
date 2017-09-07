#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define MAXLINE 64

int main(int argc, char const *argv[])
{
	int 	n, int1, int2;
	char	line[MAXLINE];
	while((n = read(STDIN_FILENO, line, MAXLINE)) > 0){
		line[n] = '\0';
		if(sscanf(line, "%d%d", &int1, &int2) == 2){
			sprintf(line, "%d\n", int1 + int2);
			n = strlen(line);
			//printf("sunlen:%d\n", n);
			if(write(STDOUT_FILENO, line, n) != n){
				perror("write to stdout error");
				return -1;
			}
		} else {
			if(line[0] == 'q'){
				printf("QUIT\n");
				return 1;
			}

			if(write(STDOUT_FILENO, "invaild arguments!\n", 19) != 19){
				perror("error invaild arguments");
				return -1;
			}
		}
	}
	return 0;
}
/*
1:add2函数将标准输入读到line中,使用sscanf函数将标准输入的转化为连个整数,相加的结果再写回line中.

st@ubuntu:~/git_project/APUE/ch15_IPC$ ./a.out 
111111 222222
sunlen:7
333333
q
QUIT

*/