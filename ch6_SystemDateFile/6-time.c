#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{
	time_t t;
	struct tm *tmp;
	char line[1024];

	if((t = time(NULL)) == -1)/* 获得日历时间 */
	{
		perror("time error");
		return -1;
	}

	if((tmp = localtime(&t)) == NULL)/* 日历时间转struct tm结构 */
	{
		perror("localtime error");
		return -1;
	}

	if(strftime(line, 1024, "%a %b %d %X %Z %Y\n", tmp) == 0)/* 高可读性输出 */
	{
		perror("strftime error");
		return -1;
	}
	fputs(line, stdout);
	return 0;
}
/*result:

	st@ubuntu:~/git_project/APUE/6$ ./a.out 
	Tue Jul 18 23:01:59 CST 2017

*/