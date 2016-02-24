/*************************************************************************
	> File Name: 04-16_12MyInitSocketFunction.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Mon 22 Feb 2016 12:25:47 AM CST
 ************************************************************************/

#include "apue.h" 
#include<errno.h>
#include<sys/socket.h>

int initseverBad(int type,const struct sockaddr *addr,socklen_t  alen,int qlen)
{
    int fd;
    int err=0;
    if((fd=socket(addr->sa_family,type,0))<0)
        return (-1);
    if(bind(fd,addr,alen)<0)
        goto errout;
    if(type==SOCK_STREAM||type==SOCK_SEQPACKET)
    {
        if(listen (fd,alen)<0)
            goto errout;
    }
    return (fd);
errout:
    err=errno;
    close(fd);
    errno=err;
    return(-1);
}
//虽然C语言警告我们不要无缘无故的使用goto因为，不安全，但是如果你把c吃透，偶尔的使用也是不错的。
//在linux系统内核中，也使用了大量的goto语句。
//但是对于现代语言系统中goto被摒弃是确定的，而且在一些熟知的socket模型中goto也在进来那个避免少的使用
//
//
//TCP由于一些奇怪的地址复用规则，使的这个例子不完备。
int initsever(int type,const struct sockaddr *addr,socklen_t  alen,int qlen)
{
    int fd,err;
    int reuse=1;
    if((fd=socket(addr->sa_family,type,0))<0)
        return (-1);
    if(setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int))<0)
        goto errout;
    if(bind(fd,addr,alen)<0)
        goto errout;
    if(type==SOCK_STREAM||type==SOCK_SEQPACKET)
    {
        if(listen (fd,alen)<0)
            goto errout;
    }
    return (fd);
errout:
    err=errno;
    close(fd);
    errno=err;
    return(-1);
}
