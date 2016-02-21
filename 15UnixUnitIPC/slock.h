/*************************************************************************
	> File Name: slock.h
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Sun 21 Feb 2016 04:12:26 PM CST
 ************************************************************************/

#ifndef _SLOCK_H
#define _SLOCK_H
#include<semaphore.h>


struct slock
{
    sem_t   *semp;
    char    name[_POSIX_NAME_MAX];
};
#endif
