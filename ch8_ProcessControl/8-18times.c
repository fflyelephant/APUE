#include <stdio.h>
#include <errno.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/wait.h>	/* for WIFXXX */

/*
	pr_exit函数打印进程的退出状态
	WIFXXX宏得到子进程返回值的状态，判断出退出方式。
*/
void pr_exit(int status)
{
	if(WIFEXITED(status))//正常退出
		printf("normal termination, exit status=%d\n", WEXITSTATUS(status));
	else if(WIFSIGNALED(status))//异常终止
		printf("abnormal termination, signal number=%d, %s\n", 
			WTERMSIG(status), 
#ifdef WCOREDUMP
			WCOREDUMP(status) ? "got a core file":" no core file"
#endif	
			);
	else if(WIFSTOPPED(status))//子进程停止
		printf("child was stopped, signal number=%d\n", WSTOPSIG(status));
}


static void pr_times(clock_t real, struct tms *tmsstart, struct tms *tmsend)
{
	static long clktck;
	if((clktck = sysconf(_SC_CLK_TCK)) < 0)// 获得每秒钟的CPU滴答数, _SC_CLK_TCK宏定义在unistd.h文件中
	{
		perror("sysconf error");
		return;
	}

	printf("   real:  %7.2f\n", real/(double)clktck); //总滴答数/每秒滴答数 = 秒数
	printf("   user:  %7.2f\n", 
	(tmsend->tms_utime - tmsstart->tms_utime)/(double)clktck);
	printf("   sys :  %7.2f\n",
	(tmsend->tms_stime - tmsstart->tms_stime)/(double)clktck);
	printf("   child user:  %7.2f\n", 
	(tmsend->tms_cutime - tmsstart->tms_cutime)/(double)clktck);
	printf("   child sys :  %7.2f\n", 
	(tmsend->tms_cstime - tmsstart->tms_cstime)/(double)clktck);
}


static void do_cmd(const char *cmd)
{
	struct tms tmsstart, tmsend;
	clock_t start, end;
	int status;

	printf("\nCommand:%s\n", cmd);
	if((start = times(&tmsstart)) < 0)
	{
		perror("times tmsstart error");
		return;
	}

	if((status = system(cmd)) < 0)
	{
		perror("cmd error");
		return;
	}

	if((end = times(&tmsend)) < 0)
	{
		perror("times tmsend error");
		return;
	}

	pr_times(end-start, &tmsstart, &tmsend);
	pr_exit(status);
}


int main(int argc, char const *argv[])
{
	int i;
	printf("argc:%d\n", argc);
	setbuf(stdout, NULL);

	for(i = 1; i < argc; i++)
		do_cmd(argv[i]);
	return 0;
}
/*result:

	st@ubuntu:~/git_project/APUE/ch8_ProcessControl$ ./a.out "sleep 5" "date" "man bash >/dev/null"

	Command:sleep 5
	   real:     5.05  -->sleep 5 命令实际执行时长是5.05秒
	   user:     0.00
	   sys :     0.00
	   child user:     0.02
	   child sys :     0.01
	normal termination, exit status=0

	Command:date
	Tue Jul 25 23:06:02 CST 2017
	   real:     0.05  -->date命令执行时长是0.05秒
	   user:     0.00
	   sys :     0.00
	   child user:     0.02
	   child sys :     0.02
	normal termination, exit status=0

	Command:man bash >/dev/null
	   real:     0.90  -->man bash >/dev/null命令执行时长是0.9秒
	   user:     0.00
	   sys :     0.00
	   child user:     0.14
	   child sys :     0.62
	normal termination, exit status=0



*/