#include <stdio.h>
#include <errno.h>
#include <string.h>
#define BUFFSIZE 48

int main(int argc, char const *argv[])
{
	FILE *fp;
	char buf[BUFFSIZE];

	memset(buf, 'a', BUFFSIZE);
	if((fp = fmemopen(buf, BUFFSIZE, "w+")) == NULL)/* fmemopen的“w+”模式打开会将文件长度清零(O_TRUNC) */
	{
		perror("fmemopen error");
		return -1;
	}
	/* first */
	printf("initial buffer contents: %s\n", buf);
	fprintf(fp, "hello world");
	printf("before fflush: %s\n", buf);
	fflush(fp);
	printf("after  fflush: %s\n", buf);
	printf("length buf: %ld\n", (long)strlen(buf));

	/* second */
	memset(buf, 'b', BUFFSIZE);
	fprintf(fp, "hello world");
	printf("before fseek: %s\n", buf);
	fseek(fp, 0, SEEK_SET);
	printf("after  fseek: %s\n", buf);
	printf("length buf: %ld\n", (long)strlen(buf));

	/* third */
	memset(buf, 'c', BUFFSIZE);
	fprintf(fp, "hello world");
	//fclose(fp);
	fflush(fp);
	printf("after fclose: %s\n", buf);
	//fclose(fp);
	printf("length buf: %ld\n", (long)strlen(buf));
	return 0;
}
/*result:
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch5_StandardIO$ ./a.out 
	initial buffer contents: 
	before fflush: 
	after  fflush: hello world
	length buf: 11
	before fseek: bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb
	after  fseek: bbbbbbbbbbbhello world
	length buf: 22
	after fclose: hello worldccccccccccccccccccccccccccccccccccccc
	length buf: 48
 */