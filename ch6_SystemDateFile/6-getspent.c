#include <stdio.h>
#include <shadow.h>
/*
	struct spwd {
		char *sp_namp;       //登录名
		char *sp_pwdp;       //加密之后的密码
		long int sp_lstchg;
		long int sp_min;
		long int sp_max;
		long int sp_warn;
		long int sp_inact;
		long int sp_expire;
		long unsigned int sp_flag;
	}
*/
/* /etc/shadow的访问需要超级权限 */
int main(int argc, char const *argv[])
{
	struct spwd *sp;
	setspent();
	while((sp = getspent()) != NULL)
	{
		if(0 ==strcmp("st", sp->sp_namp))
			printf("name: %s\tpwd: %s\tlast-pwd-change: %ld\tsp_min: %ld\tsp_max: %ld\tsp_warn: %ld\tsp_inact: %ld\tsp_expire: %ld\tsp_flag: %ld\n", \
			sp->sp_namp, sp->sp_pwdp, sp->sp_lstchg, sp->sp_min, sp->sp_max, sp->sp_warn, sp->sp_inact, sp->sp_expire, sp->sp_flag);
	}

	endspent();
	return 0;
}
/*result:

	st@ubuntu:~/git_project/APUE/6$ sudo ./a.out 
	name: st	pwd: $6$8k.c2g6g$X27H6txrHdL0CioRWCZL0F6t7O4jfBcRkrTMvcca6HWC7ZzWYxJo6vbNmMVDT14FHPXAupOAluca7KxjU9pLs.	last-pwd-change: 17365	sp_min: 0	sp_max: 99999	sp_warn: 7	sp_inact: -1	sp_expire: -1	sp_flag: -1

*/