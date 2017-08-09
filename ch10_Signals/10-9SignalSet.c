#include <signal.h>
#include <errno.h>

//注:假设目前的信号又31个，从1-31，且int类型值是32位的，正好每一位可以表示一个信号
//在上面的条件下实现sigemptyset,sigfillset,sigaddset,sigdelset,sigismember五个信号集处理函数

#define sigemptyset(ptr)  (*(ptr) == 0) //清空信号集
#define sigfillset(ptr)   (*(ptr) = ~(int)0, 0) //将信号集全设置为1

#define BADSIG(signo) ((signo) <= 0 || (signo) >= 32) //信号的值范围应该在1-31之间

//设置某一位信号
int sigaddset(int *set, int signo)
{
	if(BADSIG(signo)){
		errno = EINVAL;
		return -1;
	}

	*set |= 1 << (signo -1);
	return 0; 
}

//清除某一位信号
int sigdelset(int *set, int signo)
{
	if(BADSIG(signo)){
		errno = EINVAL;
		return -1;
	}

	*set &= 1 << (signo -1);
	return 0;
}

//测试某一位信号
int sigismember(int *set, int signo)
{
	if(BADSIG(signo)){
		errno = EINVAL;
		return -1;
	}

	return((*set &= (1 << (signo -1))) != 0)
}