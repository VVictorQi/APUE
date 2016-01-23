/*************************************************************************
	> File Name: 9-11_14MyReadWriteLock.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Sat 23 Jan 2016 06:08:37 PM CST
 ************************************************************************/
//读写锁和互斥量类似，但是拥有很好的并发性特征
#include "apue.h" 
#include<stdlib.h>
#include<pthread>

struct job
{
    struct      job*j_next;
    struct      job*j_prev;
    pthread_t   j_id;//这里只是写了请求队列的一个结构，其中结构中所带的数据在这里不做要求
};


struct queue
{
    struct      *q_head;
    struct      *q_tail;
    pthread_t   q_lock;
};



int queue_init(struct queue*qp)
{
    int err;
    qp->q_head=NULL;
    qp->q_tail=NULL;
    if((err=pthread_rwlock_init(&qp->q_lock,NULL))!=0)
        return (err);
    //这之后是初始化队列的 数据的，在这里只是搭了一个框架
    return 0;

}
//添加一个任务到队列中
void job_insert(struct queue *qp,struct job *jp)
{
    pthread_rwlock_wlock(&qp->q_lock);
    jp->j_next=NULL;
    jp->j_prev=qp->q_tail;
    if(jp->q_tail!=NULL)
        qp->q_tail->j_next=jp;
    qp->q_tail=jp;

    pthread_rwlock_unlock(&qp->q_lock);


}
//从队列中删除一个任务
void job_remove(struct queue*qp,struct job *jp)
{
    pthread_rwlock_wrlock(&qp->q_lock);
    if(jp==qp->q_head)
    {
        qp->q_head=jp->j_next;
        if(qp->q_tail==jp)
            qp->q_tail=NULL;
        else
            jp->j_next->j_prev=jp->j_prev;
    }
    else if(jp==qp->q_tail)
    {
        qp->q_tail=jp->j_prev;
        jp->j_prev->j_next=jp->j_next;
    }
    else
    {
        jp->j_prev->j_next=jp->j_next;
        jp->j_next->j_prev=jp->prev;
    }
    pthread_rwlock_unlock(&qp->q_lock);
}

//通过线程id在任务队列中查找此任务

struct job *job_find(struct queue*qp,pthread_t id)
{
    struct job*jp;
    if(pthread_rwlock_rlock(&qp->q_lock)!=0)
        return NULL;
    for(jp=qp->q_head;jp!=NULL;jp=jp->j_next)
        if(pthread_equal(jp->j_id,id))
            break;
    pthread_rwlock_unlock(&qp->q_lock);
    return jp;
}



