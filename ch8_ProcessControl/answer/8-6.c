#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#define CMD "ps -aux |grep a.out"

int main(int argc, char const *argv[])
{
	
	pid_t pid;

	if((pid = fork()) < 0){
		perror("fork error");
		return -1;
	}else if(pid == 0){//child
		printf("child id:%d\n", getpid());
		_exit(0);
	}
	printf("father id:%d\n", getpid());
	sleep(6);
	system(CMD);
	return 0;
}
/*

	st@ubuntu:~/git_project/APUE/ch8_ProcessControl/answer$ ./a.out 
	father id:7478
	child id:7479
	Warning: bad ps syntax, perhaps a bogus '-'? See http://procps.sf.net/faq.html
	st        7478  0.0  0.0   2008   280 pts/0    S+   21:54   0:00 ./a.out
	st        7479  0.0  0.0      0     0 pts/0    Z+   21:54   0:00 [a.out] <defunct> -->子进程7479的标志是Z+，表示僵尸进程
	st        7481  0.0  0.0   2236   536 pts/0    S+   21:54   0:00 sh -c ps -aux |grep a.out

*/