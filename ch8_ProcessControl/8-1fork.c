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
	if((pid = fork()) < 0 )
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

	if(waitpid(pid, &status, 0) < 0)
	{
		perror("waitpid error");
		return -1;
	}
	printf("father global_var_addr:%p  value:%d\n", &global_var, global_var);
	printf("father  local_var_addr:%p  value:%d\n", &local_var, local_var);

	return 0;
}
/*result:
//子进程的增量并没有影响到父进程，因为fork函数为子进程做了一份拷贝，我一直以为既然做了拷贝那么子进程中同一
//变量的地址应该不一样，测试却是一样的，我猜可能是书上说的拷贝到虚拟内存系统中，应该是常驻内存的意思吧，
//变量在disk还是只有一份

	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch8_ProcessControl$ ./a.out 
	before fork! 
	child global_var_addr:0x804a034  value:2
	 child local_var_addr:0xbfa82120  value:2
	father global_var_addr:0x804a034  value:1
	father  local_var_addr:0xbfa82120  value:1
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch8_ProcessControl$ ./a.out >123
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch8_ProcessControl$ cat 123
	before fork!-->exit(0)输出
	child global_var_addr:0x804a034  value:2
	 child local_var_addr:0xbf8aed50  value:2
	before fork!-->return 0输出
	father global_var_addr:0x804a034  value:1
	father  local_var_addr:0xbf8aed50  value:1
//当运行在交互模式时，行缓冲"before fork"遇见'\n'立即输出，所以只输出一次，
//而重定向到文件时是全缓冲，"before fork\n"暂存在缓冲区中，fork时此缓冲区内容被子进程复制了一份，当子进程exit
//退出时就会输出"before fork"，加上父进程return退出又输出一次。
*/

