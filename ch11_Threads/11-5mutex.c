#include <stdio.h>
#include <pthread.h>

struct foo {
	int 			f_count;
	int				f_id;
	pthread_mutex_t f_lock;
	/* more infomation */ 
};

struct foo *foo_alloc(int id)
{
	struct foo *fp;
	if((fp = malloc(sizeof(struct foo))) != NULL){
		fp->f_count = 1;
		fp->f_id	= id;
		if(pthread_mutex_init(fp->f_lock, NULL) != 0){
			free(fp);
			return NULL;
		}
	}
	return fp;
}

void foo_hold(struct foo *fp)
{
	pthread_mutex_lock(&fp->f_lock);
	fp->f_count++;
	pthread_mutex_unlock(&fp->f_lock);
}

void foo_rele(struct foo *fp)
{
	pthread_mutex_lock(&fp->f_lock);
	if(--fp->f_count == 0){
		pthread_mutex_unlock(&fp->f_lock);
		pthread_mutex_destroy(&fp->f_lock);
		free(fp);
	} else {
		pthread_mutex_unlock(&fp->f_lock);
	}
}
/*

1:当一个线程要访问结构体时,先调用foo_hold增加引用计数
2:当线程停止访问结构体时时,调用foo_rele减少引用计数,如果此时引用计数为0,释放结构体堆内存并destory互斥量
3:问题--若当有一个线程正在foo_hold阻塞等待互斥锁时,这时拥有互斥锁的线程在foo_rele中将--fp->f_count==0,并释放了锁和结构体堆内存,这是回到
  第一个线程执行fp->f_cout++将会产生不可预料的错误
*/