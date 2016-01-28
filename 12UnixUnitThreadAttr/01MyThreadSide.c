/*************************************************************************
	> File Name: 01MyThreadSide.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Tue 26 Jan 2016 04:55:31 PM CST
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

static void *func(void *p)
{
    puts("Thread is working.");
    pthread_exit(NULL);
}

int main()
{
    pthread_t tid;
    int err, i;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    // 修改每个线程的栈大小
    pthread_attr_setstacksize(&attr,1024*1024);
    for(i = 0 ; ; i++)
    {
        // 测试当前进程能创建多少个线程
        err = pthread_create(&tid,&attr,func,NULL);
        if(err)
        {    
            fprintf(stderr,"pthread_create():%s\n",strerror(err));
            break;
        }
    }
    printf("i = %d\n",i);
    pthread_attr_destroy(&attr); 
    exit(0);
}
