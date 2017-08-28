#include <stdio.h>
#include <time.h>
#include <sys/select.h>
#include <unistd.h>

unsigned mysleep(unsigned nsec)
{	
	int n;
	unsigned slept;
	time_t start, end;
	struct timeval tv;

	tv.tv_sec = nsec;
	tv.tv_usec = 0;
	time(&start);
	n = select(0, NULL, NULL, NULL, &tv);
	if(n == 0)
		return 0;

	time(&end);
	slept = end - start;
	if(slept >= nsec)
		return 0;
	return(nsec - slept);
}

int main(int argc, char const *argv[])
{
	mysleep(5);
	return 0;
}