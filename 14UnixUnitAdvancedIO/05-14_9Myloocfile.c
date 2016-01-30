/*************************************************************************
	> File Name: 05-14_9Myloocfile.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Sat 30 Jan 2016 04:04:22 PM CST
 ************************************************************************/

#include "apue.h" 
//守护进程可以通过该函书在文件上加锁
//一种简便的方法定义lookfile就是通过write_lock函数定义
//#define  lockfile(fd)   write_lock ((fd),0,SEEK_SET,0)


int lookfile(int fd)
{
    struct flock fl;
    fl.l_type=F_WRLCK;
    fl.l_start=0;
    fl.l_whence=SEEK_SET;
    fl.l_len=0;
    return (fctnl(fd,F_SETCK,&fl));
}

