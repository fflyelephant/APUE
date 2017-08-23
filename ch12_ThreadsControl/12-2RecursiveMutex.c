#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutexattr_t mutex_attr;
pthread_mutex_t     mutex;

struct to_info{
    void            (*to_fn)(void *);
    void                     *to_arg;
    struct timespec          to_wait;// 保存到过期时间的间隔
};

static void object_fun(void *arg)
{
    printf("%d\n", __LINE__);
    pthread_mutex_lock(&mutex);
    printf("%d\n", __LINE__);
    pthread_mutex_lock(&mutex);
    printf("%d\n", __LINE__);
    printf("%s\n", (char *)arg);
    pthread_mutex_unlock(&mutex);
    pthread_mutex_unlock(&mutex);
}

static void *timeout_helper(void *arg)
{
    int ret;
    struct to_info *tip;
    tip = (struct to_info*)arg;
    printf("clock_nanosleep will be wait -->sec:%ld nsec:%ld\n", tip->to_wait.tv_sec,\
    tip->to_wait.tv_nsec);
    ret = clock_nanosleep(CLOCK_REALTIME, 0, &(tip->to_wait), NULL);// 纳秒级别的sleep
    (*tip->to_fn)(tip->to_arg);
    free(arg);
    return 0;
}

static int makedetachpthread(void *(*fn)(void *), void *arg)
{
    int             err;
    pthread_t       tid;
    pthread_attr_t  attr;
    err = pthread_attr_init(&attr);
    if(err != 0){
        return err;
    }
    err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if(err == 0)
        pthread_create(&tid, &attr, fn, arg);
    pthread_attr_destroy(&attr);// 这里对destroy的返回值没有检查,默认是能够正确回收attr的资源
    return err;
}

static void timeout(const struct timespec *when, void (*func)(void *), void *arg)
{
    struct timespec now;
    struct to_info *tip;
    int             err;

    clock_gettime(CLOCK_REALTIME, &now);
    if((when->tv_sec > now.tv_sec) || ((when->tv_sec > now.tv_sec) && (when->tv_nsec > now.tv_nsec))){
          //时间未到期
          tip = (struct to_info*)malloc(sizeof(struct to_info));
          if(tip != NULL){
            tip->to_fn = func;
            tip->to_arg = arg;
            tip->to_wait.tv_sec = when->tv_sec - now.tv_sec;
            if(when->tv_nsec >= now.tv_nsec)
                tip->to_wait.tv_nsec = when->tv_nsec - now.tv_sec;
            else{
                tip->to_wait.tv_sec--;
                tip->to_wait.tv_nsec = 100000000 + when->tv_nsec - now.tv_nsec;
            }
          }
          err = makedetachpthread(timeout_helper, (void *)tip);
          if(err == 0)
                return;
          else
            free(tip);
        return;
    }
    //时间到期,直接执行func
    (*func)(arg);
}

int main(int argc, char const *argv[])
{
    int err;
    char *arg = "hello world!";
    struct timespec when;
    /* 互斥锁属性对象初始化 */
    if((err = pthread_mutexattr_init(&mutex_attr)) != 0){
        perror("pthread_mutexattr_init error");
        return -1;
    }
    /* 互斥锁添加PTHREAD_MUTEX_RECURSIVE属性 */
    if((err = pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE)) != 0){
        perror("pthread_mutexattr_settype error");
       return -1;
    }
    /* 互斥锁对象初始化(将互斥锁的PTHREAD_MUTEX_RECURSIVE属性加到互斥锁上) */
    if((err = pthread_mutex_init(&mutex, &mutex_attr)) != 0){
        perror("pthread_mutex_init error");
        return -1;
    }
    pthread_mutex_lock(&mutex);
    clock_gettime(CLOCK_REALTIME, &when);
    when.tv_sec += 6;// 到期时间在10秒后
    timeout(&when, object_fun, (void *)arg);
    pthread_mutex_unlock(&mutex);
    sleep(10);
    return 0;
}
/*
    本例子是典型的使用互斥锁PTHREAD_MUTEX_RECURSIVE属性的例子,在object_fun函数(线程入口)中可以多次加锁不
出现死锁,但当将116,117行互换之后,object_fun函数的首次加锁就会死锁,因为PTHREAD_MUTEX_RECURSIVE属性只针对同
一线程有效,互换后是先主进程加锁,后线程加锁会造成死锁(即使锁属性为PTHREAD_MUTEX_RECURSIVE)

stone@cdWSCMPL07:~/test_my/github_test/APUE/ch12_ThreadsControl$ ./a.out 
clock_nanosleep will be wait -->sec:5 nsec:99997500

--> nanosleep 5s 99997500ns

21
23
25
hello world!

--> sleep(10)进程等待线程的退出

stone@cdWSCMPL07:~/test_my/github_test/APUE/ch12_ThreadsControl$ 
*/