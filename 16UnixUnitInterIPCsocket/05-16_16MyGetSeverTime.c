/*************************************************************************
	> File Name: 05-16_16MyGetSeverTime.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Mon 22 Feb 2016 12:55:53 AM CST
 ************************************************************************/

#include "apue.h" 
#include<netdb.h>
#include<errno.h>
#include<sys/socket.h>

#define    MAXSLEEP     128
#define    BUFSIZE      128


int  connect_retry(int domain,int type,int protocol,const struct sockaddr*addr,socklen_t alen)
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

void  print_uptime(int sockfd)
{
    int     n;
    char    buf[BUFSIZE];

    while((n=recv(sockfd,buf,BUFSIZE,0))>0)
        write(STDOUT_FILENO,buf,n);
    if(n<0)
        err_sys("recv  error");
}

int main(int argc,char ** argv)
{
    struct  addrinfo        *alist,*aip;
    struct  addrinfo        hint;
    int                     sockfd,err;
    if(argc!=2)
        err_quit("usage: ruptime hostname");
    memset(&hint,0,sizeof(hint));//申请内存
    hint.ai_socktype=SOCK_STREAM;
    hint.ai_canonname=NULL;
    hint.ai_addr=NULL;
    hint.ai_next=NULL;
    if((err=getaddrinfo(argv[1],"ruptime",&hint,&alist))!=0)
        err_quit("getaddrinfo error %s",gai_strerror(err));
    for(aip=alist;aip!=NULL;aip=aip->ai_next)
    {
        if((sockfd=connect_retry(aip->ai_family,SOCK_STREAM,0,aip->ai_addr,aip->ai_addrlen))<0)
            err=errno;
        else
        {
            print_uptime(sockfd);
            exit(0);
        }
    }
    err_exit(err,"can't connect to %s",argv[1]);
}
