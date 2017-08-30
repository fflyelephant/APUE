#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

static int set_fl(int fd, int FL)
{
	int val;
	if((val = fcntl(fd, F_GETFL)) < 0)
		return -1;

	val |= FL;
	return(fcntl(fd, F_SETFL, val));
}

static int  clr_fl(int fd, int FL)
{
	int val;
	if((val = fcntl(fd, F_GETFL)) < 0)
		return -1;

	val &= ~O_NONBLOCK;
	return(fcntl(fd, F_SETFL, val));
}

char buf[500];

int main(int argc, char const *argv[])
{
	int 	ntowrite, nwrite;
	char	*ptr;

	ntowrite = read(STDIN_FILENO, buf, sizeof(buf));
	fprintf(stderr, "read %d bytes\n", ntowrite);

	set_fl(STDOUT_FILENO, O_NONBLOCK);
	ptr = buf;
	while(ntowrite > 0){
		errno = 0;
		nwrite = write(STDOUT_FILENO, ptr, ntowrite);
		fprintf(stderr, "nwrite=%d, errno=%d\n", nwrite, errno);
		if(nwrite > 0){
			ptr += nwrite;
			ntowrite -= nwrite;
		}
	}

	clr_fl(STDOUT_FILENO, O_NONBLOCK);
	return 0;
}