#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <pwd.h>
#include <signal.h>
#include <string.h>

static void my_alarm(int signo)
{
	struct passwd *root_ptr;
	printf("in signal handler\n");
	if((root_ptr = getpwnam("root")) == NULL)
	{
		perror("getpwnam root error");
		return;
	}
	alarm(1);
}


int main(int argc, char const *argv[])
{	
	struct passwd *ptr;
	signal(SIGALRM, my_alarm);
	alarm(1);
	for( ; ; ){
		if((ptr = getpwnam("st")) == NULL){
			perror("getpwnam error");
			return -1;
		}

		if(strcmp(ptr->pw_name, "st") != 0)
			printf("return value corrupted!, pw_name=%s\n", ptr->pw_name);

	}
	return 0;
}