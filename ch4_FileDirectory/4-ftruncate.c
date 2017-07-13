#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
int main(int argc, char const *argv[])
{
	int fd;
	char str[] = "hello\nstone\n";
	if((fd = open("./foo", (O_RDWR | O_CREAT | O_TRUNC), 0666)) < 0)
	{
		perror("open error");
		return -1;
	}

	if(write(fd, str, sizeof(str)) != sizeof(str))
	{
		perror("write error");
		return -1;
	}
#if 1
	/* 截断动作超过文件大小会造成文件空洞 */
	if(ftruncate(fd, 2*sizeof(str)) < 0)
	{
		perror("ftruncate error");
		return -1;
	}
#else
	/* 截断文件 */
	if(ftruncate(fd, 2) < 0)
	{
		perror("ftruncate error");
		return -1;
	}
#endif
	close(fd);
	return 0;
}
/*result1:

	st@ubuntu:~/git_project$ od -c ./foo 
	0000000   h   e   l   l   o  \n   s   t   o   n   e  \n  \0  \0  \0  \0
	0000020  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  --> 这里的文件空洞为何多了两个字节？？
	0000032

  result2:

    st@ubuntu:~/git_project$ od -c ./foo 
	0000000   h   e  --> 截断后只剩两个字节
	0000002

*/
