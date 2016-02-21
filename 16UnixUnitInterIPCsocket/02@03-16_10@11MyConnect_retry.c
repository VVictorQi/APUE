/*************************************************************************
	> File Name: 02@03-16_10@11MyConnect_retry.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Sun 21 Feb 2016 11:48:31 PM CST
 ************************************************************************/

#include "apue.h" 
#include<sys/socket.h>
#define   MAXSLEEP  128


//整个行为都是不具有跨平台的，虽然在linux和Solarias下可以，但是在基于BSD下是不可行的。
int connect_retry(int sockfd,const  struct  sockaddr *addr,socklen_t alen)
{
    int   numsec;
    for(numsec=1;numsec<=MAXSLEEP;numsec<<=1)
    {
        if(connect(sockfd,addr,alen)==0)
            return 0;
        if(numsec<=MAXSLEEP/2)
            sleep(numsec);
    }
    return (-1);
}
//因此如果connect失败，可迁移的应用程序需要关闭套接字，如果想重试，必须打开一个新的套接字。因此有了下面这个新的
//函数
int  connect_retryExtern(int domain,int type,int protocol,const struct sockaddr*addr,socklen_t alen)
{
    int numsec,fd;

    for(numsec=1;numsec<=MAXSLEEP;numsec<<=1)
    {
        if((fd=socket(domain,type,protocol))<0)
            return(-1);
        if(connect(fd,addr,alen)==0)
            return(fd);
        close(fd);

        if(numsec<=MAXSLEEP/2)
            sleep(numsec);
    }
    return (-1);
}
