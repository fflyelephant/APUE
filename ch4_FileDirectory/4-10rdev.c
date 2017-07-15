#include <stdio.h>
#include <errno.h>
#ifdef 	SOLARIS
#include <sys/mkdev.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	int i;
	struct stat st;
	if(argc<2)
	{
		printf("need some file\n");
		return -1;
	}

	for(i=1; i<argc; i++)
	{
		if(lstat(argv[i] ,&st) < 0)
		{
			perror("lstat error");
			continue;
		}

		printf("file:%s\t %d/%d \t", argv[i], major(st.st_dev), minor(st.st_dev));
		/* 如果是字符或块设备文件(只有它们的st.st_rdev属性不为空) */
		if(S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode))
			printf("(%s)--%d/%d", S_ISBLK(st.st_mode) ? "block" : "character", major(st.st_rdev), minor(st.st_rdev));
	
		putchar('\n');
	}
	return 0;
}
/*result:

	st@ubuntu:~/git_project/APUE/ch4_file-directory$ ./a.out ./ ../  /dev/tty[123]
	file:./	         8/1 	
	file:../	     8/1 	
	file:/dev/tty1	 0/5 	(character)--4/1
	file:/dev/tty2	 0/5 	(character)--4/2
	file:/dev/tty3	 0/5 	(character)--4/3

*/