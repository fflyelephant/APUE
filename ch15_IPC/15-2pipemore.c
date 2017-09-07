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
	char line[4096] = {0};
	FILE *fp;
	if(argc != 2){
		printf("usage: a.out <filename>\n");
		return -1;
	}

	if((fp = fopen(argv[1], "r")) == NULL){
		perror("fopen error");
		return -1;
	}

	if(pipe(fd) < 0){
		perror("pipe error");
		return -1;
	}

	if((pid = fork()) < 0){
		perror("fork error");
		return -1;
	} else if (pid > 0){
		close(fd[0]);
		while(fgets(line, sizeof(line), fp) != NULL){
			if(write(fd[1], line, sizeof(line)) != sizeof(line)){
				perror("write to pipe error");
				return -1;
			}
		}
		close(fd[1]);
		waitpid(pid, NULL, 0);
	} else {// child
		close(fd[1]);
		if(fd[0] != STDIN_FILENO){
			if(dup2(fd[0], STDIN_FILENO) != STDIN_FILENO){
				printf("dup2 error to stdin");
				return -1;
			}
			close(fd[0]);
		}
		printf("PAGER:%s\n", getenv("PAGER"));
		if(execl("/bin/more", "/bin/more", (char*)0) < 0){
			perror("execl error");
			return -1;
		}
		exit(0);
	}
	return 0;
}