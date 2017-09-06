#include <stdio.h>

/* select 实现sleep_us */
void sleep_us(unsigned int usecs)
{
	struct timeval  tptr;
	tptr.tv_sec = usecs / 1000000;
	tptr.tv_usec = (usecs - (1000000 * tptr.tv_sec));
	select(0, NULL, NULL, NULL, tvptr);
}

/* poll  实现sleep_us */
void sleep_us(unsigned int usecs)
{
	struct pollfd fdarray;
	int timeout;
	if((timeout = usecs/1000) <= 0)
		timeout = 1;

	poll(&fdarray, 0, timeout);
}