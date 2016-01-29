/*************************************************************************
	> File Name: 01-14_1MyNoBlockIo.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Fri 29 Jan 2016 11:46:01 AM CST
 ************************************************************************/

#include "apue.h" 
#include<errno.h>
#include<fcntl.h>
char buf[50000];

void clr_fl(int fd,int flags)
{
    int val;
    if((val=fcntl(fd,F_GETFL,0))<0)
        err_sys("fcntl F_GETFL error");
    val = ~flags;
    if(fcntl(fd,F_SETFL,val)<0)
        err_sys("fcntl F_SETFL error");
}
void set_fl(int fd,int flags)
{
    int val;
    if((val=fcntl(fd,F_GETFL,0))<0)
        err_sys("fcntl F_GETFL error");
    val |=flags;
    if(fcntl(fd,F_SETFL,val)<0)
        err_sys("fcntl F_SETFL error");
}

int main(void)
{
    int     ntowrite ,nwrite;
    char    *ptr;


    ntowrite=read(STDIN_FILENO,buf,sizeof(buf));
    fprintf(stderr,"read %d bytes\n ",ntowrite);
    set_fl(STDOUT_FILENO,O_NONBLOCK);  //设置为非阻塞
    
    ptr=buf;
    while(ntowrite>0)
    {
        errno=0;
        nwrite=write(STDOUT_FILENO,ptr,ntowrite);
        fprintf(stderr,"nwrite=%d,errno=%d\n",nwrite,errno);

        if(nwrite>0)
       {
            ptr+=nwrite;
            ntowrite-=nwrite;
        }
    }
    clr_fl(STDOUT_FILENO,O_NONBLOCK);//清除非阻塞状态
    exit(0);
}
