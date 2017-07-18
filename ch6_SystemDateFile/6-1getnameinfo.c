#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <pwd.h>

int main(int argc, char const *argv[])
{
	struct passwd *ptr;
	setpwent();
	while((ptr = getpwent()) != NULL)
		printf("name:%s\tpasswd:%s\tuid:%d\tgid:%d\treal name:%s\tdir:%s\tshell:%s\n", ptr->pw_name, \
			ptr->pw_passwd, ptr->pw_uid, ptr->pw_gid, ptr->pw_gecos, ptr->pw_dir, ptr->pw_shell);
	endpwent();
	return 0;
}