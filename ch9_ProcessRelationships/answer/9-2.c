#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define ERROR_EXIT(m)\
do\
{\
    perror(m);\
    exit(EXIT_FAILURE);\
}\
while(0);
	
static void pr_ids(const char *name)
{
	printf("%s: pid:%ld, ppid:%ld, pgrp:%ld, tpgrp:%ld\n", \
		    name, (long)getpid(), (long)getppid(), (long)getpgrp(), (long)tcgetpgrp(STDIN_FILENO));
	fflush(stdout);
}

int main(int argc, char const *argv[])
{
	pid_t pid;
	if((pid = fork()) < 0){
		ERROR_EXIT("fork error");
	}else if(pid > 0){// parent exit
		sleep(3);
		exit(EXIT_SUCCESS);
	}

	pr_ids("child1");
	if(setsid() < 0)// 子进程成为新的会话组长
		ERROR_EXIT("setsid error");
	pr_ids("child2");
	return 0;
}

/*

	st@ubuntu:~/git_project/APUE/ch9_ProcessRelationships/answer$ ./a.out 
	child1: pid:4142, ppid:4141, pgrp:4141, tpgrp:4141 -->在调用setsid之前，进程组ID是4141是父进程ID
	child2: pid:4142, ppid:4141, pgrp:4142, tpgrp:-1  -->进程组ID是子进程ID4142，表示子进程成为了新的进程组长，并且控制终端拥有的前台进程组为-1
													   //表示新的会话没有控制终端了

*/