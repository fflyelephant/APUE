#include <stdio.h>
#include <errno.h>
#include <sys/resource.h>

#define doit(name) pr_limits(#name, name)

static void pr_limits(char *name, int resource)
{
	struct rlimit      limit;
	unsigned long long lim;
	if(getrlimit(resource, &limit) < 0)
	{
		printf("getrlimit for %s error!\n", name);
		return;
	}
	printf("%-14s ", name);

	/* 资源软限制 */
	if(limit.rlim_cur == RLIM_INFINITY){
		printf("(infinity) ");
	} else {
		lim = limit.rlim_cur;
		printf("%12lld ", lim);
	}

	/* 资源硬限制 */
	if(limit.rlim_max == RLIM_INFINITY){
		printf("(infinity) ");
	} else {
		lim = limit.rlim_max;
		printf("%12lld ", lim);
	}

	printf("\n");
}

int main(int argc, char const *argv[])
{
#ifdef RLIMIT_AS
	doit(RLIMIT_AS);
#endif
	/* 平台通用 */
	doit(RLIMIT_CORE);
	doit(RLIMIT_CPU);
	doit(RLIMIT_DATA);
	doit(RLIMIT_FSIZE);
	doit(RLIMIT_NOFILE);
	doit(RLIMIT_STACK);

#ifdef RLIMIT_MEMLOCK
	doit(RLIMIT_MEMLOCK);
#endif

#ifdef RLIMIT_MSGQUEUE
	doit(RLIMIT_MSGQUEUE);
#endif

#ifdef RLIMIT_NICE
	doit(RLIMIT_NICE);
#endif

#ifdef RLIMIT_NPROC
	doit(RLIMIT_NPROC);
#endif
		
#ifdef RLIMIT_NPTS
	doit(RLIMIT_NPTS);
#endif

#ifdef RLIMIT_RSS
	doit(RLIMIT_RSS);
#endif

#ifdef RLIMIT_SBSIZE
	doit(RLIMIT_SBSIZE);
#endif

#ifdef RLIMIT_SIGPENDING
	doit(RLIMIT_SIGPENDING);
#endif

#ifdef RLIMIT_SWAP
	doit(RLIMIT_SWAP);
#endif

#ifdef RLIMIT_VMEM
	doit(RLIMIT_VMEM);
#endif
	return 0;
}
/*result:

	stone@cdWSCMPL07:~/test_my/github_test/APUE/ch7_ProcessEnvironment$ ./a.out 
	RLIMIT_AS      (infinity) (infinity) 
	RLIMIT_CORE             0 (infinity) 
	RLIMIT_CPU     (infinity) (infinity) 
	RLIMIT_DATA    (infinity) (infinity) 
	RLIMIT_FSIZE   (infinity) (infinity) 
	RLIMIT_NOFILE        1024      65536 
	RLIMIT_STACK      8388608 (infinity) 
	RLIMIT_MEMLOCK      65536      65536 
	RLIMIT_MSGQUEUE    819200     819200 
	RLIMIT_NICE             0          0 
	RLIMIT_NPROC        32016      32016 
	RLIMIT_RSS      (infinity) (infinity) 
	RLIMIT_SIGPENDING    32016      32016 

*/