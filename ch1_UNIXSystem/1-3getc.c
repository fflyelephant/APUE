#include <stdio.h>
int main(int argc, char const *argv[])
{
	int c;
	while((c = getc(stdin)) != EOF)
		if(putc(c, stdout) == EOF)
		{
			perror("putc error");
			return -1;
		}
	if(ferror(stdin))
		perror("getc error");
	return 0;
}