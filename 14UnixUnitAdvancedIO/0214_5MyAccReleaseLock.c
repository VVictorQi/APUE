/*************************************************************************
	> File Name: 0214_5MyAccReleaseLock.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Sat 30 Jan 2016 02:27:19 PM CST
 ************************************************************************/

#include "apue.h" 
#include<fcntl.h>
int lock_reg(int fd,int cmd,int type,off_t offset ,int whence , off_t len)
{
    struct flock lock;


    lock.l_type=type;
    lock.l_start=offset;
    lock.l_whence=whence;
    lock.l_len=len;

    //为避免每次分配flock结构的繁琐，将其动作封装成函数，一起运用，提高效率
    //，在以后会用到
    return fcntl(fd,cmd,&lock);
}

pid_t lock_test(int fd,int type,off_t offset ,int whence ,off_t len)
{   
    //函数的作用是测试一把锁，如果存在一把锁，它阻塞由参数指定的锁请求，则此函数返回持有
    //这把锁的进程的进程ID，否则此函数返回0。
    struct flock lock;
    lock.l_type=type;
    lock.l_start=offset;
    lock.l_whence=whence;
    lock.l_len=len;
    if(fcntl(fd,F_GETLK,&lock)<0)//利用fcntl添加记录锁，并且记录状态
        err_sys("fcntl error");
    if(lock.l_type==F_UNLCK)
        return 0;
    return (lock.l_pid);
}



