int makedetachpthread(void *(*fn)(void *), void *arg)
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