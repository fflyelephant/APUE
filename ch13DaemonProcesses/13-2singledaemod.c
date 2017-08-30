#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#define LOCKFILE "/var/run/daemon.pid"
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

/* 文件记录锁锁定整个文件内容同一时刻只允许一个进程更改文件内容 */
static int lockfile(int fd)
{
	struct flock fl;
	fl.l_type 	= F_WRLCK;
	fl.l_start	= 0;
	fl.l_whence	= SEEK_SET;
	fl.l_len	= 0;
	return(fcntl(fd, F_SETLK, &fl));
}

int already_running(void)
{
	int 	fd;
	char	buf[16];

	fd = open(LOCKFILE, O_RDWR|O_CREAT, LOCKMODE);
	if(fd < 0){
		syslog(LOG_ERR, "can't open %s:%s", LOCKFILE, strerror(errno));
		exit(1);
	}

	if(lockfile(fd) < 0){
		if(errno == EACCES || errno == EAGAIN){
			close(fd);
			return 1;
		}
		syslog(LOG_ERR, "can't lock %s:%s", LOCKFILE, strerror(errno));
		exit(1);
	}

	ftruncate(fd, 0);
	sprintf(buf, "%ld", (long)getpid());
	write(fd, buf, strlen(buf)+1);
	return 0;
}

/*
1:仅允许daemon的一个副本操作LOCKFILE文件写入pid,其它的daemon副本会在lockfile时返回EACCES或者EAGAIN

*/