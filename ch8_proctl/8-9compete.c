#include <stdio.h>		/* for convenience */
#include <unistd.h>

void output(const char *str)
{
	char *ptr;
	int c;
	setbuf(stdout, NULL);/* 设置无IO缓冲 */
	for(ptr=str; (c=*ptr++) != 0; )
		putc(c, stdout);
}

/* 竞争的现象不明显，而且父进程是始终先运行的 */
int main(int argc, char const *argv[])
{
	pid_t pid;
	if((pid = fork()) < 0)
		printf("fork error\n");
	else if(pid == 0)
		output("form child process\n");
	else
		output("from father process\n");
	return 0;
}
