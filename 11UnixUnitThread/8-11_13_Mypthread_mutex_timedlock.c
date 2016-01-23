/*************************************************************************
	> File Name: 8-11_13_Mypthread_mutex_timedlock.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Sat 23 Jan 2016 05:19:17 PM CST
 ************************************************************************/

#include "apue.h" 
#include<pthread.h>


int main(void)
{
    int err;
    struct timespec tout;
    struct tm*tmp;
    char buf[64];
    pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;


    pthread_mutex_lock(&lock);
    printf("mutex is lockerd\n");
    clock_gettime(CLOCK_REALTIME,&tout);
    tmp=localtime(&tout.tv_sec);
    strftime(buf,sizeof(buf),"%r",tmp);
    printf("current time is %s\n",buf);


    tout.tv_sec+=10;
    err=pthread_mutex_timedlock(&lock,&tout);
    clock_gettime(CLOCK_REALTIME,&tout);
    tmp=localtime(&tout.tv_sec);

    strftime(buf,sizeof(buf),"%r",tmp);
    printf("the time is now %s\n",buf);

    if(err==0)
        printf("mutex locked again!\n");
    else
        printf("can't lock mutex again: %s \n",strerror(err));
    exit(0);

}
