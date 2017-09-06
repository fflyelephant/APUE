#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

static int set_fl(int fd, int FL)
{
	int val;
	if((val = fcntl(fd, F_GETFL)) < 0)
		return -1;

	val |= FL;
	return(fcntl(fd, F_SETFL, val));
}

int main(int argc, char const *argv[])
{
	int i, n;
	int fd[2];
	if(pipe(fd) < 0){
		perror("pipe error");
		return -1;
	}

	set_fl(fd[1], O_NONBLOCK);// 设置写端非阻塞
	/* 一次写一个字节到fd[1],直到pipe被填满 */
	for(n=0; ;n++){
		if(write(fd[1], "a", 1) != 1){
			perror("write error");
			break;
		}
	}
	printf("pipe size:%d\n", n);
	return 0;
}
/*
stone@cdWSCMPL07:~/test_my/github_test/APUE/ch14_AdvancedIO/answer$ ./a.out 
write error: Resource temporarily unavailable
pipe size:65536
本平台(Ubuntu 5.4.0-6ubuntu1~16.04.4)的pipe大小为:65535

*/