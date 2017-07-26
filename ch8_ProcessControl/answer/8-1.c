#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int global_var = 1;
int main(int agrc, char *argv[])
{
	int local_var = 1;
	int status;
	int pid;
	int ret;
	printf("before fork!\n");
	if((pid = fork()) < 0 )
	{
		perror("fork error");
		return -1;
	}
	else if(pid == 0)
	{
		global_var++;
		local_var++;
		printf("child global_value:%d  local_value:%d\n", global_var, local_var);
		fclose(stdout);// 子进程显式关闭stdout
		exit(0);
	}

	if(waitpid(pid, &status, 0) < 0)
	{
		perror("waitpid error");
		return -1;
	}
	ret = printf("father global_value:%d  local_value:%d\n", global_var, local_var);
	printf("%d\n", ret);
	return 0;
}
/*result：

	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch8_ProcessControl/answer$ ./a.out 
	before fork!
	child global_value:2  local_value:2
	father global_value:1  local_value:1  -->还是有输出，和平台的具体实现有关
	37

*/