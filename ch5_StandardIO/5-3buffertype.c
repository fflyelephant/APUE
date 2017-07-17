#include <stdio.h>
#include <errno.h>
#include <unistd.h>

/* 文件指针的缓冲区类型和长度在不同平台有不同的接口，这部分是不可移植的 */
#if defined(_IO_UNBUFFERED)
int is_unbuffered(FILE *fp)
{
	return (fp->_flags & _IO_UNBUFFERED);
}

int is_linebuffered(FILE *fp)
{
	return (fp->_flags & _IO_LINE_BUF);
}

int buffer_size(FILE *fp)
{
	return (fp->_IO_buf_end - fp->_IO_buf_base);
}
#elif defined(__SNBF)
int is_unbuffered(FILE *fp)
{
	return (fp->_flags & __SNBF);
}

int is_linebuffered(FILE *fp)
{
	return (fp->_flags & __SLBF);
}

int buffer_size(FILE *fp)
{
	return (fp->_bf._size);
}
#elif defined(_IONBF)
#ifdef _LP64
#define _flag __pad[4]
#define _ptr  __pad[1]
#define _base __pad[2]
#endif

int is_unbuffered(FILE *fp)
{
	return (fp->_flag & _IONBF);
}

int is_linebuffered(FILE *fp)
{
	return (fp->_flag & _IOLBF);
}

int buffer_size(FILE *fp)
{
#ifdef _LP64
	return (fp->_base - fp->_ptr);
#else
	return (BUFSIZ);
#endif
}
#else
#error unknown stdio implementation!
#endif


/* 打印fp的缓冲类型及缓冲区的长度 */
void pr_stdio(const char *name, FILE *fp)
{
	printf("stream : %s\t", name);
	if(is_unbuffered(fp))
		printf("unbuffered!\t");
	else if(is_linebuffered(fp))
		printf("linebuffered\t");
	else
		printf("fullbuffered\t");
	printf("buffsize:%d\n", buffer_size(fp));
}	


int main(int argc, char const *argv[])
{
	FILE *fp;
	fputs("enter any character\n", stdout);
	if(getchar() == EOF)
	{
		perror("getchar got a EOF");
		return -1;
	}	

	fputs("one line output to stderr\n", stderr);

	pr_stdio("stdin", stdin);
	pr_stdio("stdout", stdout);
	pr_stdio("stderr", stderr);

	if((fp = fopen("/etc/passwd", "r")) == NULL)
	{
		perror("fopen error");
		return -1;
	}

	if(getc(fp) == EOF)
	{
		printf("/etc/passwd is empty!\n");
		if(fp) fclose(fp);
		return -1;
	}

	pr_stdio("/etc/passwd", fp);		
	fclose(fp);
	return 0;
}
/*result:

	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch5_StandardIO$ ./a.out 
	enter any character
	hello										 
	one line output to stderr
	stream : stdin  linebuffered    buffsize:1024 	-->stdin,stdout连接终端都是行缓冲，stderr是不缓冲的
	stream : stdout linebuffered    buffsize:1024
	stream : stderr unbuffered!     buffsize:1
	stream : /etc/passwd    fullbuffered    buffsize:4096  --> 普通文件的读写默认都是全缓冲
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch5_StandardIO$ ./a.out < /etc/group > std.out 2>std.err
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch5_StandardIO$ cat std.out 
	enter any character
	stream : stdin  fullbuffered    buffsize:4096	-->stdin,stdout重定向到文件时是全缓冲，stderr仍然是不缓冲
	stream : stdout fullbuffered    buffsize:4096
	stream : stderr unbuffered!     buffsize:1
	stream : /etc/passwd    fullbuffered    buffsize:4096
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch5_StandardIO$ cat std.err 
	one line output to stderr

*/