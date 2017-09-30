#include <stdio.h>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define NQ 		3 		/* 消息队列的个数 */
#define KEY 	0x123	/* 首个消息队列的key值 */
#define MAXLEN	512		/* 消息的最大长度 */

struct threadinfo{
	int		qid;
	int 	fd;
};

struct mymesg{
	long 	mtype;
	char	mtext[MAXLEN];
};

void sig_int(int signo)
{
	printf("\nGot a SIGINT and exit!\n");
	exit(1);
}

/* 线程从队列获得消息内容,写到套接字的写端 */
void *helper(void *arg)
{
	int 					 n;
	struct mymesg 			 m;
	struct threadinfo *tip = arg;
	while(1){
		memset(&m, 0, sizeof(struct mymesg));
		if((n = msgrcv(tip->qid, &m, MAXLEN, 0, MSG_NOERROR)) < 0){
			perror("msgrcv error");
			return;
		}
		if(write(tip->fd, m.mtext, n) < 0){
			perror("write error");
			return;
		}
	}
}

int main(int argc, char const *argv[])
{
	int 				i, n;
	int 				fd[2];
	int 				qid[NQ];
	struct pollfd 		pfd[NQ];
	struct threadinfo	ti[NQ];
	pthread_t 			tid[NQ];
	char 				buf[MAXLEN];

	signal(SIGINT, sig_int);

	for(i = 0; i < NQ; i++){
		if((qid[i] = msgget(KEY+i, IPC_CREAT|0666)) < 0){
			perror("msgget error");
			return -1;
		}
		printf("Number %d quene, id is:%d\n", i, qid[i]);
		/* 创建一对已连接套接字 */
		if(socketpair(AF_UNIX, SOCK_DGRAM, 0, fd) < 0){
			perror("socketpair error");
			return -1;
		}	
		/* 检测读端 */
		pfd[i].fd = fd[0];
		pfd[i].events = POLLIN;

		ti[i].qid = qid[i];
		ti[i].fd  = fd[1];
		/* 为每个队列创建一个线程(传递队列id和套接字写端) */
		if(pthread_create(&tid[i], NULL, helper, &ti[i]) != 0){
			perror("pthread_create error");
			return -1;
		}
	}
	while(1){
		if(poll(pfd, NQ, -1) < 0){
			perror("poll error");
			return -1;
		}
		/* 从套接字的读端读到消息队列的内容 */
		for(i = 0; i < NQ; i++){
			if(pfd[i].revents & POLLIN){
				if((n = read(pfd[i].fd, buf, sizeof(buf))) < 0){
					perror("read error");
				}
				buf[n] = '\0';
				printf("queue id:%d, Got message:%s\n", qid[i], buf);
			}
		}
	}
	return 0;
}

/*
1:因为消息队列的id不属于文件描述符系列,所以不能使用select or poll函数实现IO复用, 本程序使用socketpair创建的一对已连接套接字
来实现IO复用, 将消息队列的消息复制到套接字中来实现IO复用
2:消息经历过程
   消息发出到消息队列-->消息队列接收到数据-->将消息写到fd[1]-->从fd[0]中读出消息显示

*/