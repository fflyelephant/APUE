#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

int main(int argc, char const *argv[])
{
	int 	c;

	while((c = getchar()) != EOF){
		if(isupper(c))
			c = tolower(c);
		if(putchar(c) == EOF){
			perror("putchar error");
			return -1;
		}
		if(c == '\n')
			fflush(stdout);
	}
	return 0;
}
