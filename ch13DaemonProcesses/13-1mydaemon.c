#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#define ERROR_EXIT(m)\
do\
{\
    perror(m);\
    exit(EXIT_FAILURE);\
}\
while(0)


static void daemonize(const char *cmd)
{
	int 				i, fd0, fd1, fd2;
	pid_t				pid;
	struct rlimit		rl;
	struct sigaction	sa;
	/* 清除文件掩模和改变工作目录 */
	umask(0);
	if(chdir("/") < 0)
		ERROR_EXIT("chdir error");

	/* 获取文件描述符上限并关闭所有文件描述符 */
	if(getrlimit(RLIMIT_NOFILE, &rl) < 0)
		ERROR_EXIT("getrlimit error");
	if(rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;
	for(i=0; i<rl.rlim_max; i++)
		close(i);

	/* 第一次fork父进程退出,子进程脱离当前控制终端 */
	if((pid = fork()) < 0)
		ERROR_EXIT("first fork error");
	else if(pid > 0)
		exit(0);

	/* 子进程成为会话组长 */
	setsid();

	/* 处理SIGHUP信号 */
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if(sigaction(SIGHUP, &sa, NULL) < 0)
		ERROR_EXIT("sigaction error");

	/* 第二次fork子进程退出,孙进程不再是会话组长,无权开启终端(与终端彻底隔离) */
	if((pid = fork()) < 0)
		ERROR_EXIT("second fork error");
	else if(pid > 0)
		exit(0);

	/* 将 0,1,2重定向到/dev/null */
	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	/* 初始化log file */
	openlog(cmd, LOG_CONS, LOG_DAEMON);
	if(fd0 != 0 || fd1 != 1 || fd2 != 2){
		syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
		exit(1);
	}
}

int main(int argc, char const *argv[])
{
	daemonize("hello stone");
	sleep(60);
	return 0;
}
/*

st@ubuntu:~/git_project/APUE/ch13DaemonProcesses$ ps -aux |grep a.out
Warning: bad ps syntax, perhaps a bogus '-'? See http://procps.sf.net/faq.html
st        7150  0.0  0.0   2116    80 ?        S    21:48   0:00 ./a.out
st        7157  0.0  0.0   4388   820 pts/0    S+   21:48   0:00 grep --color=auto a.out

*/