#include <stdio.h>
#include <grp.h>

/*
	struct group {
		char *gr_name;
		char *gr_passwd;
		int  gr_gid;
		char **gr_mem; //属于这个组的成员的名字将保存在指针数组指向的位置
	}
*/
int main(int argc, char const *argv[])
{
	int i = 0;
	struct group *gr;
	setgrent();
	while((gr = getgrent()) != NULL)
	{
		if(!strcmp("st", gr->gr_name))
		{
			printf("name: %s\tpasswd: %s\tgid: %d\n", gr->gr_name, gr->gr_passwd, gr->gr_gid);
			while(gr->gr_name[i++])
				printf("gr_mem[%d]: %s\n", i-1, gr->gr_mem[i-1]);
		}
	}
	endgrent();
	return 0;
}
/*result:

	st@ubuntu:~/git_project/APUE/6$ ./a.out 
	name: st	passwd: x	gid: 1000
	gr_mem[0]: (null)
	gr_mem[1]: (null)

*/