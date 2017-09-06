#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define COPYSIZE (1024*1024)  // 一次拷贝1M大小(因为程序要用它来计算off值,所以必须是4096的整数倍)

int main(int argc, char const *argv[])
{
	int 		fin, fout;
	void		*src, *dst;
	size_t 		copysz;
	struct stat	sbuf;
	off_t 		fsz = 0;
	printf("page_size:%d\n", getpagesize());

	if(argc != 3){
		printf("usage: %s <fromfile> <tofile>\n", argv[0]);
		return -1;
	}

	if((fin = open(argv[1], O_RDWR)) < 0){
		perror("open error");
		return -1;
	}

	if((fout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0777)) < 0){
		perror("open error");
		return -1;
	}

	if(fstat(fin, &sbuf) < 0){
		perror("fstat error");
		return -1;
	}

	if(ftruncate(fout, sbuf.st_size) < 0){
		perror("ftruncate error");
		return -1;
	}
	printf("total size:%ld\n", sbuf.st_size);
	while(fsz < sbuf.st_size){
		if((sbuf.st_size - fsz) > COPYSIZE)
			copysz = COPYSIZE;
		else
			copysz = sbuf.st_size -fsz;
		printf("copysz:%d fsz:%ld\n", copysz, fsz);
		if((src = mmap(NULL, copysz, PROT_READ, MAP_SHARED, fin, fsz)) == MAP_FAILED){
			perror("map fin error");
			return -1;
		}
		
		printf("copysz:%d fsz:%ld\n", copysz, fsz);
		if((dst = mmap(NULL, copysz, PROT_WRITE, MAP_SHARED, fout, fsz)) == MAP_FAILED){
			perror("map fin error");
			return -1;
		}

		memcpy(dst, src, copysz);
		munmap(src, copysz);
		munmap(dst, copysz);
		fsz += copysz;
		printf("\tremain size:%ld bytes\n", sbuf.st_size - fsz);		
	}
	return 0;
}

/*
1:原型
	void *mmap(void *addr, size_t len, int prot, int flag, int fd, off_t off);
	函数将将一个已经open的文件映射到内存中,操作内存就像操作文件本身一样,可以越过read/write来读写文件
2:参数
	addr:指定文件映射之后内存起始地址,通常指定NULL,让内核选择并返回起始地址
	 len:映射区域的长度
	prot:表示映射后内存区域的操作权限,四种取值(一般取值PROT_READ | PROT_WRITE,并且映射区域的权限不能比open文件时的权限多)
		 1:PROT_READ   --> 映射区域可读
		 2:PROT_WRITE  --> 映射区域可写
		 3:PROT_EXEC   --> 映射区域可执行
		 4:PROT_NONE   --> 映射区域不可访问
	flag:描述了映射区域的一些属性,
		 1:MAP_FIXED   --> 返回地址等于addr,如果addr=NULL,指定这个属性将造成困扰,不推荐使用,即使addr!=NULL,内核也只是将addr作为映射位置的参考,
		 最后的映射区域起始位置不一定就是addr指向的位置
		 2:MAP_SHARED  --> 表示对映射区域的修改操作同步到真实文件
		 3:MAP_PRIVATE --> 表示对映射区域的修改是存在某个副本中,不影响真实文件(MAP_SHARED和MAP_PRIVATE必须指定其一)
	  fd:已打开的文件描述符
	 off:表示映射文件的偏移量,一般为0表示从文件起始处开始映射len文件长度并且长度是pagesize的整数倍！


stone@cdWSCMPL07:~/test_my/github_test/APUE/ch14_AdvancedIO$ ls -l openswan-2.6.43.1.tar.gz 
-rw-rw-r-- 1 stone stone 8799161 Sep  6 09:47 openswan-2.6.43.1.tar.gz
stone@cdWSCMPL07:~/test_my/github_test/APUE/ch14_AdvancedIO$ ./a.out openswan-2.6.43.1.tar.gz test.tar.gz
page_size:4096
total size:8799161
copysz:1048576 fsz:0
copysz:1048576 fsz:0
        remain size:7750585 bytes
copysz:1048576 fsz:1048576
copysz:1048576 fsz:1048576
        remain size:6702009 bytes
copysz:1048576 fsz:2097152
copysz:1048576 fsz:2097152
        remain size:5653433 bytes
copysz:1048576 fsz:3145728
copysz:1048576 fsz:3145728
        remain size:4604857 bytes
copysz:1048576 fsz:4194304
copysz:1048576 fsz:4194304
        remain size:3556281 bytes
copysz:1048576 fsz:5242880
copysz:1048576 fsz:5242880
        remain size:2507705 bytes
copysz:1048576 fsz:6291456
copysz:1048576 fsz:6291456
        remain size:1459129 bytes
copysz:1048576 fsz:7340032
copysz:1048576 fsz:7340032
        remain size:410553 bytes
copysz:410553 fsz:8388608
copysz:410553 fsz:8388608
        remain size:0 bytes

*/