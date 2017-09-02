/*
头文件:
   #include <sys/select.h>
   #include <sys/time.h>
   #include <sys/types.h>
   #include <unistd.h>

原型:
	int select(int maxfd1, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *tvptr);
辅助函数:
	int FD_ZERO(fd_set *fdset); 
	int	FD_CLR(int fd, fd_set *fdset);
	int FD_SET(int fd, fd_set *fd_set);
	int FD_ISSET(int fd, fd_set *fd_set)
注:一般是先将fd_set变量清空,再添加感兴趣的fd到fd_set中

	tvptr是指定等待时间:
	tvptr = NULL : 永久等待,直到某个fd状态准备好了或者被某个信号中断
	tvptr = 0	 : 不等待,直接返回
	tvptr->tv_sec > 0 || tvptr->tv_usec > 0 :表示等待指定时间

返回值:
	-1: 错误
	 0: 超时时间到且没有准备好的fd
	>0: 表示准备好的fd数目(同一fd可读写状态都准备好计数2次)

注: select(0, NULL, NULL, NULL, tvptr)提供了微妙级别的sleep

--------------------------------------------------------------------------------------------------------

原型:
	int pselect(int maxfd1, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, const struct timespec *tvptr
					const sigset_t *sigmask);
pselect和select区别:
	1:计时时间是struct timespec是纳秒级别
	2:多了sigmask,在等待期间按照sigmask阻塞信号,返回时恢复信号屏蔽集

--------------------------------------------------------------------------------------------------------
头文件:
	#include <poll.h>

原型:
	int poll(struct pollfd fdarray[], nfds_t nfds, int milliseconds);

	milliseconds取值:
		-1: 永久等待,直到某个fd状态准备好了或者被某个信号中断
		 0: 不等待,直接返回
		>0: 表示等待指定微妙

struct pollfd {
	int 	fd;	//感兴趣的文件描述符
	short	events;//感兴趣的文件状态
	short	revents;//poll返回时设置的文件描述符状态
}

events的取值:
	//读
	POLLIN     --> 普通或优先级数据可读
	POLLRDNORM --> 普通数据可读
	POLLRDBAND --> 优先级数据可读
	POLLPRI	   --> 高优先级数据可读

	//写
	POLLOUT/POLLWRNORM	   --> 可写正常数据
	POLLWRBAND --> 可写高优先级数据

	//异常
	POLLERR    --> 错误发生
	POLLHUP	   --> 挂起发生
	POLLNVAL   --> 描述符不是一个打开的文件
注:
   1:可使用 | 来指定想观察的特性,如fdarray[0].event = POLLIN | POLLOUT, 表示对fdarray[0]中描述的可读和可写
   状态设置监听
   2:poll返回之后,可使用 & 来检测相关状态,如if(fdarray[0].revent & POLLIN)
   											{..read..}

返回值:
	-1: 错误
	 0: 超时时间到且没有准备好的fd
	>0: 表示准备好的fd数目(同一fd可读写状态都准备好计数2次)

--------------------------------------------------------------------------------------------------------

原型:
	int ppoll(struct pollfd fdarray[], nfds_t nfds,
               const struct timespec *timeout, const sigset_t *sigmask);
注:
	1:同pselect和select的差别一样,有时间精度和可设置信号屏蔽集两个差别

--------------------------------------------------------------------------------------------------------

*/ 