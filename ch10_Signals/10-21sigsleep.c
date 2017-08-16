#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
static int i;

static void sig_int(int signo)
{
	printf("i:%d\n", i);

}
int main(int argc, char const *argv[])
{
	int i;
	signal(SIGINT, sig_int);
	i = sleep(10);
	printf("i:%d\n", i);
	return 0;
}