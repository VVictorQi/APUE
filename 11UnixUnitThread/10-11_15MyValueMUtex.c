/*************************************************************************
	> File Name: 10-11_15MyValueMUtex.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Tue 26 Jan 2016 02:33:05 PM CST
 ************************************************************************/

#include<pthread.h>
struct msg
{
    struct msg*m_next;
    //附属数据
};



struct msg *workq;
pthread_cond_t qready=PTHREAD_COND_INITIALIZER;
pthread_mutex_t qlock=PTHREAD_MUTEX_INITIALIZER;



void process_msg(void)
{
    struct msg*mp;
    for(;;)//目的就是遍历整个队列
    {
        pthread_mutex_lock(&qlock);
        while(workq==NULL)
            pthread_cond_wait(&qready,&qlock);
        mp=workq;
        workq=mp->m_next;
        pthread)mutex_unlock(&qlock);
    }
}



void enqueue_msg(struct msg *mp)
{
    pthread_mutex_lock(&qlock);
    mp->m_next=workq;
    workq=mp;
    pthread_mutex_unlock(&qlock);
    pthread_cond_signal(&qready);
}
//实例给出如何使用条件变量和互斥量对线程进行同步。
//条件是工作队列的状态，我们用互斥量来霸虎条件，在while循环语句中判断条件。
//把消息放到工作队列时，需要占有互斥量，但是在给等待线程发信号时，不需要占有互斥量。
//只要线程调用pthread_cond_signal之前把消息从消息队列中脱出了就可以在释放互斥量以后完成
//这部分工作。因为我们在while循环中检车条件，所以不存在这样的问题，线程醒来，发现队列仍为空，然后返回继续等待。如果代码不能容忍这种竞争，就需要再给线程发信号的时候占领互斥量。












