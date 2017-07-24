#include <stdio.h>		/* for convenience */
#include <unistd.h>
#include <sys/types.h>

/* 初始化传递给新进程的环境变量，一个以NULL结束的指针数组 */
char *env_init[] = {"USER=unknown", "PATH=/tmp", NULL};

int main(int argc, char const *argv[])
{
	pid_t pid;
	if((pid = fork()) < 0)
		printf("fork error\n");
	else if(pid == 0)/* 在子进程中启动新进程(pid不改变) */
	{
		printf("execle pid:%ld\n", (long)getpid());
		if(execle("./echoall", "echoall", "argv[1]", "argv[2]", "argv[3]", (char *)0, env_init) < 0)/* execle 启动新进程(使用新的环境变量env_init) */
			printf("execle error\n");
	}
	if(waitpid(pid, NULL, 0) < 0)
		printf("waitpid error\n");

	if((pid = fork()) < 0)
		printf("fork error\n");
	else if(pid == 0)
	{
		printf("execlp pid:%ld\n", (long)getpid());
		if(execlp("echoall", "echoall", "argv[1]", (char *)0) < 0)/* execlp 启动新进程(沿用当前环境变量，且echoall程序要在PATH路径中找得到) */
			printf("execlp error\n");
	}	

	if(waitpid(pid, NULL, 0) < 0)
		printf("waitpid error\n");

	return 0;
}
/*result:

	st@ubuntu:~/git_project/APUE/ch8_proctl$ ./exec 
	execle pid:6041 
	echoall pid:6041 ->新进程的pid没有改变哦
	argv[0]:echoall
	argv[1]:argv[1]
	argv[2]:argv[2]
	argv[3]:argv[3]
	USER=unknown     ->execle使用了新的环境变量
	PATH=/tmp
	execlp pid:6042
	echoall pid:6042 ->新进程pid同样不变
	argv[0]:echoall
	argv[1]:argv[1]
	SSH_AGENT_PID=2064 ->execlp沿用旧的环境变量(较多)
	GPG_AGENT_INFO=/tmp/keyring-UGTERi/gpg:0:1
	.....
	.....
	XAUTHORITY=/home/st/.Xauthority
	_=./a.out

*/