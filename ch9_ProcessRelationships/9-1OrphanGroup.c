#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static void sig_hup(int signo)
{
	printf("Got SIGHUP, pid=%ld\n", (long)getpid());
}

static void pr_ids(const char *name)
{
	printf("%s: pid:%ld, ppid:%ld, pgrp:%ld, tpgrp:%ld\n", \
		    name, (long)getpid(), (long)getppid(), (long)getpgrp(), (long)tcgetpgrp(STDIN_FILENO));
	fflush(stdout);
}

int main(int argc, char const *argv[])
{
	pid_t pid;
	char c;
	pr_ids("parent");
	if((pid = fork()) < 0){
		perror("fork error");
		return -1;
	}else if(pid > 0){// parent
		sleep(3);
	}else{
		pr_ids("child");
		signal(SIGHUP, sig_hup);// 当一个进程退出导致一个进程组成为孤儿进程组，且新的孤儿进程组中所有进程处于停止状态，则SIGHUP,SIGCONT会依次
		kill(getpid(), SIGTSTP);// 送到孤儿进程组中的所有孤儿进程
		pr_ids("child");
		if(read(STDIN_FILENO, &c, 1) != 1){
			printf("read error %d on controlling TTY\n", errno);
		}
	}

	return 0;
}
/*

	st@ubuntu:~/git_project/APUE/ch9_ProcessRelationships$ ./a.out 
	parent: pid:3355, ppid:2414, pgrp:3355, tpgrp:3355
	child: pid:3356, ppid:3355, pgrp:3355, tpgrp:3355
	<停顿3秒钟>
	Got SIGHUP, pid=3356
	child: pid:3356, ppid:1, pgrp:3355, tpgrp:2414 -->子进程的父进程ID为1，表示被init进程收养。进程组3355就是一个孤儿进程组
	read error 5 on controlling TTY

什么是孤儿进程组:
	该组中的每个成员的父进程仅有两种情况:1<父进程是该组的成员> 2<父进程不属于该组所在的会话中>
	比如，若一个进程组所有成员的父进程都是init,init就满足不属于该组所在的会话，那么这个组就叫做孤儿进程组	
*/