#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

extern char **environ;
pthread_mutex_t mutex;
static pthread_once_t init_done = PTHREAD_ONCE_INIT;

static void thread_init(void)
{
	printf("thread_init\n");
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&mutex, &attr);
	pthread_mutexattr_destroy(&attr);
}

/* 可重入版本的putenv */
int myputenv_r(const char *str)
{
	int  i;
	char *str_key = NULL;
	pthread_once(&init_done, thread_init);
	pthread_mutex_lock(&mutex);
	str_key = strchr(str, '=');
	for(i=0; environ[i]!=NULL; i++){
		if(0 == strncmp(str, environ[i], str_key - str)){
			strcpy(environ[i], str);
			return 0;
		}
	}
	// 没有找到NAME对应的环境变量,将str添加到环境变量environ的末尾
	environ[i++] = str;
	environ[i] = NULL;
	pthread_mutex_unlock(&mutex);
	return 0;
}

int main(int argc, char const *argv[])
{
	char *str = "STONE=123";
	char *key = "STONE";
	char *ptr = malloc(strlen(str));
	if(ptr == NULL){
		perror("malloc error");
		return -1;
	}
	strcpy(ptr, str);
	myputenv_r(ptr);
	printf("STONE:%s\n", getenv(key));
	for(int i=0; environ[i]!=NULL; i++){
		printf("environ[%d]:%s\n", i, environ[i]);
	}
	return 0;
}
/*
stone@cdWSCMPL07:~/test_my/github_test/APUE/ch12_ThreadsControl/answer$ ./a.out 
thread_init
STONE:123
environ[0]:XDG_SESSION_ID=521
environ[1]:TERM=vt100
environ[2]:SHELL=/bin/bash
environ[3]:SSH_CLIENT=172.18.75.244 39350 22
environ[4]:SSH_TTY=/dev/pts/0
environ[5]:USER=stone
environ[6]:LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.Z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.jpg=01;35:*.jpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.m4a=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.oga=00;36:*.opus=00;36:*.spx=00;36:*.xspf=00;36:
environ[7]:SSH_AUTH_SOCK=/tmp/ssh-MukNEeuYWQ/agent.3548
environ[8]:MAIL=/var/mail/stone
environ[9]:PATH=/home/stone/bin:/home/stone/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
environ[10]:PWD=/home/stone/test_my/github_test/APUE/ch12_ThreadsControl/answer
environ[11]:LANG=en_US.UTF-8
environ[12]:SHLVL=1
environ[13]:HOME=/home/stone
environ[14]:LOGNAME=stone
environ[15]:SSH_CONNECTION=172.18.75.244 39350 172.18.53.126 22
environ[16]:LESSOPEN=| /usr/bin/lesspipe %s
environ[17]:XDG_RUNTIME_DIR=/run/user/1014
environ[18]:LESSCLOSE=/usr/bin/lesspipe %s %s
environ[19]:_=./a.out
environ[20]:OLDPWD=/home/stone/test_my/github_test/APUE
environ[21]:STONE=123
stone@cdWSCMPL07:~/test_my/github_test/APUE/ch12_ThreadsControl/answer$ 

*/