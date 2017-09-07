#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define MAXLINE 1024
int main(int argc, char const *argv[])
{
	char line[MAXLINE];
	FILE *fpin;

	if((fpin = popen("./myuclc", "r")) == NULL){
		perror("popen error");
		return -1;
	}

	for( ; ; ){
		fputs("prompt> ", stdout);
		fflush(stdout);

		if(fgets(line, MAXLINE, fpin) == NULL)
			break;
		
		if(line[0] == 'q')
			break;

		if(fputs(line, stdout) == EOF){
			perror("fputs error");
			return -1;
		}
	}

	if(pclose(fpin) == -1){
		perror("pclose error");
		return -1;
	}
	putchar('\n');
	return 0;
}
/*
1:filter只连接了myuclc的标准输出,读取标准输出并显示在stdout,myuclc的标准输入是stdin,随后的程序15-9
将add2的标准输入和输出都连接到了15-9程序上.(其实15-9完全不用连接add2的标准输入,让它就等于stdin,连接了之后只是
将add2的标准输入变成了:stdin->15-9->add2->15-9->stdout,远没有直接stdin->add2->15-9->stdout来得简单)

	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch15_IPC$ ./a.out 
	prompt> AAAABBBBCCCC
	aaaabbbbcccc
	prompt> SDFSDFSDF
	sdfsdfsdf
	prompt> Q
		--> 退出
*/