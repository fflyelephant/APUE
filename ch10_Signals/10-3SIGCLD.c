#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

static void sig_cld(int signo)
{
	pid_t pid;
	int status;

	printf("SIGCLD received!\n");
	if(signal(SIGCLD, sig_cld) == SIG_ERR)
		perror("signal error");

	if((pid = wait(&status)) < 0)
		perror("wait error");

	printf("pid:%d\n", pid);
}

int main(int argc, char const *argv[])
{
	pid_t pid;
	if(signal(SIGCLD, sig_cld) == SIG_ERR)
		perror("signal error");

	if((pid = fork()) < 0){
		perror("fork error");
		return -1;
	}else if(pid == 0){//child
		sleep(2);
		_exit(0);
	}

	pause();
	return 0;
}