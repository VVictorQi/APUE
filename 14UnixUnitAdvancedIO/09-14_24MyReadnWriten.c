/*************************************************************************
	> File Name: 09-14_24MyReadnWriten.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Mon 15 Feb 2016 09:51:24 PM CST
 ************************************************************************/

#include "apue.h" 

ssize_t readn(int fd,void *ptr ,size_t n)
{
    size_t      nleft;
    ssize_t     nread;


    nleft=n;
    while(nleft>0)
    {
        if(nread=read(fd,ptr,nleft)<0)
        {
            if(nleft==0)
                return -1;
            else 
                break;
        }
        else if(nread==0)
        {
            break;//EOF error
        }
        nleft-=nread;
        ptr+=nread;
    }
    return (n=nleft);//return >=0
}

//改进版的read 和write函数。以为传进数据，读写指定的 数据
ssize_t writen(int fd, const void * ptr ,size_t n)
{
    size_t      nleft;
    ssize_t     nwriten;

    nleft=n;

    while(nleft>0)
    {
        if(nwriten=write(fd,ptr,nleft)<0)
        {
            if(nleft==n)
                return -1;
            else
                break;
        }
        else if(nwriten==0)
            break;
        nleft-=nwriten;
        ptr+=nwriten;
    }
    return (n-nleft);
}
