/*************************************************************************
	> File Name: 6-11_11MyTwoMutex.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Sat 23 Jan 2016 02:23:55 PM CST
 ************************************************************************/

#include <stdlib.h>
#include<pthread.h>


#define NHASH 29
#define HASH(id) (((unsigned long)id)%NHASH)



struct foo *fh[NHASH];//散列列表

pthread_mutex_t hashlock=PTHREAD_MUTEX_INITIALIZER;

struct foo
{
    int                     f_count;
    pthread_mutex_t         f_lock;
    int                     f_id;
    struct foo              *f_next;
    //更多的结构体变量设定
};


strcut foo  *foo_alloc(int id)
{
    struct foo          *fp;
    int                 idx;
    if((fp=malloc(sizeof(struct foo )))!=NULL)
    {
        fp->f_count=1;
        fp->f_id=id;
        if(pthread_mutex_init(&fp->f_lock,NULL)!=0)
        {
            free(fp);
            return (NULL);
        }
        idx=HASH(id);
        pthread_mutex_lock(&hashlock);
        fp->f_next=fh[idx];
        fh[idx]=fp;
        pthread_mutex_lock(&fp->f_lock);
        pthread_mutex_unlock(&hashlock);
        pthread_mutex_unlock(&fp->f_lock);
    }
    return (fp);
}

void foo_hold(struct foo *fp)
{
    pthread_mutex_lock(&fp->f_lock);
    fp->f_count++;
    pthread_mutex_unlock(&fp->f_lock);
}


struct foo * foo_find(int id)
{
    struct foo *fp;
    pthread_mutex_lock(&hashlock);
    for(fp=fh[HASH(id)];fp!=NULL;fp->f_next)
    {
        if(fp->f_id==id)
        {
            fp->f_count++;
            break;
        }
    }
    pthread_mutex_unlock(&hashlock);
    return (fp);
}


void foo_rele(struct foo *fp)
{
    struct foo  *tfp;
    int         idx;
    pthread_mutex_lock(&fp->f_lock);
    if(--fp->f_count==0)
    {
        idx=HASH(fp->f_id);
        tfp=fh(idx);
        if(tfp==fp)
        {
            fh[idx]=fp->f_next;
        }
        else
        {
            while(tfp->f_next!=fp)
                tfp=tfp->f_next;
            tfp->f_next=fp->f_next;
        }
        pthread_mutex_unlock(&hashlock);
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
    }
    else
    {
        pthread_mutex_unlock(&fp->f_lock);
    }
}

