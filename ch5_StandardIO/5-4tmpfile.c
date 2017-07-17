#include <stdio.h>
#include <errno.h>
#define MAXLINE 1024
int main(int argc, char const *argv[])
{
	char name[L_tmpnam];/* tmpnam参数非空时，参数数组的长度至少要L_tmpnam字符 */
	char line[MAXLINE];/* 一般行缓冲长度是1024=MAXLINE */
	FILE *fp;

	printf("%s\n", tmpnam(NULL)); /* tmpnam参数为空，仅返回指向文件名的指针，文件名存储在静态区，再次调用tmpnam会更新此静态区的内容 */
	tmpnam(name);/* tmpnam只是给出一个在系统中独一无二的名字 */	
	printf("%s\n", name);
	if((fp = tmpfile()) == NULL)
	{
		perror("tmpfile error");
		return -1;
	}
	fputs("hello to a file\n", fp); /* 向临时文件输入字符串 */
	rewind(fp);						/* 调整临时文件的当前偏移量到文件首 */
	if(fgets(line, sizeof(line), fp) == NULL)
	{
		perror("fgets error");
		if(fp) fclose(fp);
		return -1;
	}
	fputs(line, stdout);
	fclose(fp);
	return 0;
}
/*result:

	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch5_StandardIO$ gcc 5-4tmpfile.c 
	/tmp/ccsRtWke.o: In function `main':
	5-4tmpfile.c:(.text+0x30): warning: the use of `tmpnam' is dangerous, better use `mkstemp'  
			不建议使用tmpnam，因为一般调用方式是使用tmpnam获得一个独一无二的文件名字，下一步就是
		使用这个名字创建这个文件，这两个步骤之前有时间间隔，在这个时间间隔期间可能其它进程就创建了这个
		文件，导致冲突发生。所以推荐使用tmpfile函数来替代
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch5_StandardIO$ ./a.out 
	/tmp/filegpKeuy
	/tmp/file1xpL9t
	hello to a file

*/