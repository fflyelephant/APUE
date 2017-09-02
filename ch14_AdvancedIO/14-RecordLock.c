/*文件记录锁
原型:
	int fcntl(int fd, int cmd, struct flock *flockptr);
	要操作文件记录锁,cmd的取值为:F_GETLK, F_SETLK, F_SETLKW
cmd:
	F_GETLK:判断由flockptr描述的锁是否在fd文件中存在,如果存在,flockptr的内容将更新为此锁的信息,若不存在这样
	的锁,除了l_type变为F_UNLCK以外,其余信息不变
	F_SETLK:给文件fd设置flockptr描述的锁,失败的话立即返回errno等于EACCES或EAGAIN,l_type=F_UNLCK可以清楚锁
	F_SETLKW:是F_SETLK的阻塞版本,如不能成功设置为导致休眠
注: 
	1:一般是用F_GETLK测试是否有这种锁,判断之后再使用F_SETLK设置锁,但这不是一个原子操作,中间可能会被其它进程
	获得锁,导致F_SETLK失败
	2:一般都是使用F_SETLK来进行锁和解锁,很少使用F_GETLK来测试文件的锁

*flockptr:
	struct flock {
		short 	l_type;
		short	l_whence;
		off_t	l_start;
		off_t	l_len;
		off_t	l_pid;
	};
	l_type:锁类型,有F_RDLCK(共享读锁), F_WRLCK(独占写锁), F_UNLCK(解锁/无锁)
	l_whence:与lseek函数的参数whence参数类似,有SEEK_SET(起始位), SEEK_CUR(当前位), SEEK_END(末位)
	l_start:与lseek函数的参数offset参数类似,可以为负,相对l_whence的位置的偏移量作为起始位置
	l_len:锁住区域的长度,为0表示锁区域长度延伸至最大值(不管以后向文件添加多少数据都会被纳入锁中)
	l_pid:记录了持有锁的进程的ID,只有cmd是F_GETLK才填写
注:
	1:要锁住整个文件,l_whence=SEEK_SET, l_start = l_len = 0;
	2:所谓共享读锁和独占写锁的概念类似线程同步中的读写锁,在这里,这个特性体现在进程之间,如果在单一进程中,
	同一进程连续两次获取独占写锁不会被阻塞,后面一次获取会替代原来的锁
	3:获取文件读锁信息,fd需要是以读打开的文件描述符,写锁类同

*/

/* 测试记录锁 */
pid_t lock_test(int fd, int type, off_t offset, int whence, off_t len)
{
	struct flock 	lock;
	lock.l_type 	= type;
	lock.l_whence	= whence;
	lock.l_start	= offset;
	lock.l_len		= len;	
	if(fcntl(fd, F_GETLK, &lock) < 0){
		perror("F_GETLK error");
		return -1;
	}
	/* 没有lock描述的锁 */
	if(lock.l_type == F_UNLCK)
		return 0;

	/* lock描述的锁已存在,返回有此锁的pid */
	return(lock.l_pid);
}

/* 测试记录锁的宏(被锁返回true) */
#define is_read_lock(fd, offset, whence, len)\
			(lock_test((fd), F_RDLCK, (offset), (whence), (len)) != 0)
#define is_write_lock(fd, offset, whence, len)\
			(lock_test((fd), F_WRLCK, (offset), (whence), (len)) != 0)
--------------------------------------------------------------------------------------------------

/* 锁和解锁记录锁 */
int	lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
	struct flock 	lock;
	lock.l_type 	= type;
	lock.l_whence	= whence;
	lock.l_start	= offset;
	lock.l_len		= len;
	return(fcntl(fd, cmd, &lock));
}

/* 设置读锁 */
#define read_lock(fd, offset, whence, len)\
			lock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
#define readw_lock(fd, offset, whence, len)\
			lock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))
/* 设置写锁 */
#define write_lock(fd, offset, whence, len)\
			lock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
#define writew_lock(fd, offset, whence, len)\
			lock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len))
/* 解锁 */
#define un_lock(fd, offset, whence, len)\
			lock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))
--------------------------------------------------------------------------------------------------

