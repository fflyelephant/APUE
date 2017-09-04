#include <stdio.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
	int fd;
	char buf1[10] = "HELLO,";
	char buf2[10] = "Mrs.tree\n";
	char ret1[10] = {0};
	char ret2[10] = {0};
	struct iovec iov[2];
	struct iovec retiov[2];
	iov[0].iov_base = buf1;
	iov[0].iov_len  = sizeof(buf1);
	iov[1].iov_base = buf2;
	iov[1].iov_len  = sizeof(buf2);

	retiov[0].iov_base = ret1;
	retiov[0].iov_len  = sizeof(ret1);
	retiov[1].iov_base = ret2;
	retiov[1].iov_len  = sizeof(ret2);

	if((fd = open("./text.txt", O_RDWR | O_CREAT, 0777)) < 0){
		perror("open error");
		return -1;
	}
	/* 将不连续内存的数据写到文件中 */
	if(writev(fd, iov, 2) < 0){
		perror("writev error");
		return -1;
	}
	close(fd);

	if((fd = open("./text.txt", O_RDWR | O_CREAT, 0777)) < 0){
		perror("open second error");
		return -1;
	}
	/* 将文件中的数据读到不连续的内存中 */
	if(readv(fd, retiov, 2) < 0){
		perror("writev error");
		return -1;
	}
	printf("retiov[0]:%s\n", (char *)retiov[0].iov_base);
	printf("retiov[1]:%s\n", (char *)retiov[1].iov_base);
	close(fd);
	return 0;
}
/*
1:原型
	ssize_t readv(int fd, const struct iovec *iov, int iovcnt);
	ssize_t writev(int fd, const struct iovec *iov, int iovcnt);

	struct iovec {
		void *iov_base;
		size_t iov_len;
	}

2:这两个高级IO将不连续空间的数据一次性写入到fd,或者一次性读出到不连续的空间中,一定程度上提高了IO的效率

st@ubuntu:~/git_project/APUE/ch14_AdvancedIO$ ./a.out 
retiov[0]:HELLO,
retiov[1]:Mrs.tree

st@ubuntu:~/git_project/APUE/ch14_AdvancedIO$ 

*/