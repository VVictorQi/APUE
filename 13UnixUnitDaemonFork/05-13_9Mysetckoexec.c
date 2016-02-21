/*************************************************************************
	> File Name: 05-13_9Mysetckoexec.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Mon 22 Feb 2016 01:53:37 AM CST
 ************************************************************************/

#include "apue.h" 
#include<fcntl.h>
int set_cloexec(int fd)
{
    int val;
    if((val=fcntl(fd,F_GETFD,0))<0)
        return -1;
    val |=FD_CLOEXEC;
    return(fcntl(fd,F_SETFD,val));
}
