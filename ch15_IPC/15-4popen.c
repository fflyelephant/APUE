#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 64
#define PAGER "${PAGER:-more}"

int main(int argc, char const *argv[])
{
	char 	line[MAXLINE];
	FILE	*fpin, *fpout;

	if(argc != 2){
		printf("usage: a.out <filename>\n");
		return -1;
	}

	if((fpin = fopen(argv[1], "r")) == NULL){
		perror("fopen error");
		return -1;
	}

	if((fpout = popen(PAGER, "w")) == NULL){
		perror("popen for write error");
		goto out;
	}

	while(fgets(line, MAXLINE, fpin) != NULL){
		if(fputs(line, fpout) == EOF){
			perror("fputs error");
			goto out;
		}
	}

out:
	if(fpin) fclose(fpin);
	if(pclose(fpout) == -1){
		perror("pclose error");
		return -1;
	}	
	return 0;
}