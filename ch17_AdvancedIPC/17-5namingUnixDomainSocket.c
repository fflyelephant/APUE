#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stddef.h>
#include <string.h>
#include <sys/un.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
	int fd, size;
	struct sockaddr_un un;
	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, "foo.socket");
	if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0){
		perror("socket error");
		return -1;
	}
	/*
		offsetof函数返回sun_path在结构体struct sockaddr_un中的偏移量, 因为struct sockaddr_un结构体在各个平台
	中的定义不同, 所以成员sun_path之前的内容跨平台而异, strlen是sun_path的实际长度(不含末尾\0)
	offsetof是一个宏定义, 如下:
		#define  offsetof(TYPE, MEMBER)  ((int)&((TYPE *)0)->MEMBER)
	*/
	size = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
	if(bind(fd, (struct sockaddr *)&un, size) < 0){
		perror("bind error");
		return -1;
	}
	return 0;
}
/*
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch17_AdvancedIPC$ ls -l
	total 12
	-rw-rw-r-- 1 stone stone  601 Sep 24 16:27 17-5namingUnixDomainSocket.c
	-rwxrwxr-x 1 stone stone 7528 Sep 24 16:27 a.out
	srwxrwxr-x 1 stone stone    0 Sep 24 16:28 foo.socket
	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch17_AdvancedIPC$ ./a.out 
	bind error: Address already in use
*/