#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

const char *getfiletype(mode_t mode)
{
	if(0 == mode)
		return "mode_t has error";
	if(S_ISREG(mode))
		return "regular";
	else if(S_ISDIR(mode))
		return "directory";
	else if(S_ISCHR(mode))
		return "character special";
	else if(S_ISBLK(mode))
		return "block special";
	else if(S_ISFIFO(mode))
		return "fifo";
	else if(S_ISLNK(mode))
		return "link";
	else if(S_ISSOCK(mode))
		return "socket";
	else
		return "** unknow type **";
}

int read_directory(const char *directory)
{
	DIR *dp = NULL;
	struct dirent *dirp = NULL;
	struct stat st;
	mode_t mymode;
	char filename[1024] = {0};
	char *ptr = NULL;
	strncpy(filename, directory, 1024);
	if(lstat(directory ,&st) < 0)
	{
		perror("lstat error");
		return -1;
	}

	mymode = st.st_mode;
	if(S_ISDIR(st.st_mode)){/* 是目录就继续读 */
		ptr = filename + strlen(filename);/* ptr保存了添加子目录/文件的位置 */
		if(*(ptr-1) != '/') *ptr++ = '/';/* 父子目录之间的'/'符号 */
		printf("Directory:%s\t--type:%s\n", filename, getfiletype(mymode));
		if((dp = opendir(filename)) == NULL){
			perror("opendir error");
			return -1;
		}	
		while((dirp = readdir(dp)) != NULL){
			if(strcmp(dirp->d_name, ".")==0 || strcmp(dirp->d_name, "..")==0)/* 忽略. 和 .. 目录，否则会造成重复 */
				continue;
			strncpy(ptr, dirp->d_name, sizeof(dirp->d_name));
			read_directory(filename);
		}
		closedir(dp);
	}
	else{/* 是一个文件就打印文件名字 */
		printf("--file:%s\t--type:%s\n", filename, getfiletype(mymode));
	}
}



/* 给一个目录，输出目录下所有文件(不跟随链接文件) */
int main(int argc, char const *argv[])
{
	if(argc != 2)
	{
		printf("need a directory\n");
		return -1;
	}

	read_directory(argv[1]);
	return 0;
}
/*result:

	st@ubuntu:~/git_project/APUE/ch4_file-directory$ ./a.out ./
	Directory:./	            --type:directory
	--file:./4-5link.c	        --type:regular
	--file:./a.out	            --type:regular
	--file:./4-6timesave.c	    --type:regular
	--file:./4-8chdir.c	        --type:regular
	--file:./4-ftruncate.c	    --type:regular
	--file:./4-7readdirectory.c	--type:regular
	--file:./4-2access.c	    --type:regular
	--file:./4-1filetype.c    	--type:regular
	--file:./4-3umask.c	        --type:regular
	--file:./4-4chmod.c  	    --type:regular
	--file:./softlink	        --type:link

*/