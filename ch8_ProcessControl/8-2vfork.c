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
	printf("before fork!\n");
	if((pid = vfork()) < 0 )
	{
		perror("fork error");
		return -1;
	}
	else if(pid == 0)
	{
		global_var++;
		local_var++;
		printf("child global_var_addr:%p  value:%d\n", &global_var, global_var);
		printf(" child local_var_addr:%p  value:%d\n", &local_var, local_var);
		exit(0);
	}

	printf("father global_var_addr:%p  value:%d\n", &global_var, global_var);
	printf("father  local_var_addr:%p  value:%d\n", &local_var, local_var);

	return 0;
}
/*result:
//vfork保证子进程先运行，而且子进程使用的是父进程的地址空间，所以在子进程中修改的变量会影响到父进程

	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch8_ProcessControl$ ./a.out 
	before fork!
	child global_var_addr:0x804a030  value:2
	 child local_var_addr:0xbfc23684  value:2
	father global_var_addr:0x804a030  value:2
	father  local_var_addr:0xbfc23684  value:2
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch8_ProcessControl$ ./a.out >123
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch8_ProcessControl$ cat 123 
	before fork!
	child global_var_addr:0x804a030  value:2
	 child local_var_addr:0xbfd8af84  value:2
	father global_var_addr:0x804a030  value:2
	father  local_var_addr:0xbfd8af84  value:2

*/

