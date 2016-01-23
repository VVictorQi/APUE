/*************************************************************************
	> File Name: 5-11_10MyPthreadMutex_t.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Sat 23 Jan 2016 12:20:26 PM CST
 ************************************************************************/

#include <stdlib.h>
#include<pthread.h>

struct foo
{
    int                 f_count;
    pthread_mutex_t     f_lock;
    int                 f_id;
    //......更多的结构体变量
}
struct foo * foo_alloc(int id)
{
    if((fp=malloc(sizeof(struct foo)))!=NULL)
    {
        fp->f_count=1;
        fp->f_id=id;
        if(pthread_mutex_init(&fp->f_lock,NULL)!=0)
        {
            free(fp);
            return NULL;
        }
    }
    return fp;
}



void foo_hold(struct foo *fp )
{
    pthread_mutex_lock(&fp->f_lock);
    fp->f_count++;
    pthread_mutex_unlock(&fp->f_lock);
}


void foo_rele(struct foo *fp)
{
    pthread_mutex_lock(&fp->f_lock);
    if(--fp->f_count ==0)
    {
        pthread_mutex_unlock(fp->f_lock);
        pthread_mutex_destory(fp->f_lock);
        free(fp);
    }
    else
    {
        pthread_mutex_unlock(&fp->f_lock);
    }
}
