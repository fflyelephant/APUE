#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <error.h>

#define MAXLEN 512

struct mymesg{
	long mtype;
	char mtext[MAXLEN];
};

int main(int argc, char const *argv[])
{
	key_t 		   key;
	long  		   qid;
	size_t		   nbytes;
	struct  mymesg m;

	key = strtol(argv[1], NULL, 0);
	if((qid = msgget(key, 0)) < 0){
		perror("msgget error");
		return -1;
	}

	memset(&m, 0, sizeof(m));
	strncpy(m.mtext, argv[2], MAXLEN-1);
	nbytes = strlen(m.mtext);
	m.mtype = 1;
	if(msgsnd(qid, &m, nbytes, 0) < 0){
		perror("msgsnd error");
		return -1;
	}
	return 0;
}
/*
1:在pollmsg程序运行后生成三个消息队列, id分别是0x123, 0x124, 0x125, sendmsg分别向这三个队列发送消息

./pollmsg:
st@ubuntu:~/git_project/APUE/ch17_AdvancesIPC$ ./pollmsg
Number 0 quene, id is:32769
Number 1 quene, id is:0
Number 2 quene, id is:65538
queue id:32769, Got message:hello1
queue id:0, Got message:hello2
queue id:65538, Got message:hello3
^C
Got a SIGINT and exit!

./sendmsg:
st@ubuntu:~/git_project/APUE/ch17_AdvancesIPC$ ./sendmsg 0x123 hello1
st@ubuntu:~/git_project/APUE/ch17_AdvancesIPC$ ./sendmsg 0x124 hello2
st@ubuntu:~/git_project/APUE/ch17_AdvancesIPC$ ./sendmsg 0x125 hello3
st@ubuntu:~/git_project/APUE/ch17_AdvancesIPC$ ./sendmsg 0x126 hello4
msgget error: No such file or directory

*/
