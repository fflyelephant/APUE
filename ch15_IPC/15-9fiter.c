#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define MAXLINE 1024

static void sig_pipe(int signo)
{
	printf("caught SIGPIPE\n");
	exit(0);
}

int main(int argc, char const *argv[])
{
	int 	n, fd1[2], fd2[2];
	pid_t	pid;
	char 	line[MAXLINE];

	if(signal(SIGPIPE, sig_pipe) == SIG_ERR){
		perror("signal error");
		return -1;
	}

	if(pipe(fd1) < 0 || pipe(fd2) < 0){
		perror("some pipe error");
		return -1;
	}

	if((pid = fork()) < 0){
		perror("fork error");
		return -1;
	} else if (pid > 0){//father fd1[1]写,fd2[0]读
		//关闭fd1读端和fd2写端
		close(fd1[0]);
		close(fd2[1]);
		//每次从标准输入读入待add2处理的数据
		while(fgets(line, MAXLINE, stdin) != NULL){
			n = strlen(line);
			//向fd1写数据,add2将从fd1[0]那里读出,fd1[0]使用dup2成为了add2的标准输入
			if(write(fd1[1], line, n) != n){
				perror("write to fd1[1] error");
				return -1;
			}

			//从fd2读出数据,fd2[1]作为了add2的标准输出,接收add2的处理结果,从这里的fd2[0]中读出结果显示
			if((n = read(fd2[0], line, MAXLINE)) < 0){
				perror("read from fd2[0] error");
				return -1;
			}
			if(n == 0){
				printf("child close pipe\n");
				break;
			}
			line[n] = '\0';
			//输出add2处理结果
			if(fputs(line, stdout) == EOF){
				perror("fputs error");
				return -1;
			}
		}
		if(ferror(stdin))
			perror("stdin error");
		exit(0);
	} else {// child, execl执行add2前,将进程的标准输入连接到fd1[0],标准输出连接到fd2[1]
		close(fd1[1]);
		close(fd2[0]);
		//将add2的标准输入连接到fd1[0]
		if(fd1[0] != STDIN_FILENO){
			if(dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO){
				perror("dup2 error to stdin");
				return -1;
			}
			close(fd1[0]);
		}
		//将add2的标准输出连接到fd2[1]
		if(fd2[1] != STDOUT_FILENO){
			if(dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO){
				perror("dup2 error to stdin");
				return -1;
			}
			close(fd2[1]);
		}

		if(execl("./add2", "add2", (char *)0) < 0){
			perror("execl error");
		}
		exit(0);
	}
}
/*
	st@ubuntu:~/git_project/APUE/ch15_IPC$ ./a.out 
	11 22
	33
	222
	invaild arguments!
	12 21
	33
	q
	QUIT

	caught SIGPIPE -->当只write to fd[1]没有进程read from fd[0]是会产生SIGPIPE信号
*/


