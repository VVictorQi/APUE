/*************************************************************************
	> File Name: 06-_17MyPostClientTime.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Mon 22 Feb 2016 01:25:47 AM CST
 ************************************************************************/

#include "apue.h" 
#include<netdb.h>
#include<errno.h>
#include<syslog.h>
#include<sys/socket.h>
#include"./04-16_12MyInitSocketFunction.c"
#define     BUFSIZE  128
#define     QLEN     10
#ifndef     HOST_NAME_MAX
#define     HOST_NAME_MAX   256
#endif





extern  int initsever(int,const struct sockaddr*,socklen_t,int);

void  sever(int sockfd)
{
    int     clfd;
    FILE    *fp;
    char    buf[BUFSIZE];

    set_cloexec(sockfd);
    for(;;)
    {
        if((clfd=accept(sockfd,NULL,NULL))<0)
        {
            syslog(LOG_ERR,"ruptime :accept error:%s",strerror(errno));
            exit(1);
        }
        set_cloexec(clfd);
        if((fp=popen("/usr/bin/uptime","r"))==NULL)
        {
            sprintf(buf,"error:%s\n",strerror(errno));
            send(clfd,buf,strlen(buf),0);
            pclose(fp);
        }
        close(clfd);

    }
}

int  main(int argc,char ** argv)
{
    struct   addrinfo       *alist,*aip;
    struct   addrinfo       hint;
    int                     sockfd,err,n;
    char                    *host;


    if(argc!=1)
        err_sys("usage:ruptime");
    if((n=sysconf(_SC_HOST_NAME_MAX))<0)
        n=HOST_NAME_MAX;
    if((host=malloc(n))==NULL)
        err_sys("malloc error");
    if(gethostname(host,n)<0)
        err_sys("gethostname error");
    daemonize("ruptimed");
    memset(&hint,0,sizeof(hint));
    hint.ai_flags=AI_CANONNAME;
    hint.ai_socktype=SOCK_STREAM;
    hint.ai_canonname=NULL;
    hint.ai_addr=NULL;
    hint.ai_next=NULL;
    if((err=getaddrinfo(host,"ruptime",&hint,&alist))!=0)
    {
        syslog(LOG_ERR,"ruptimed:getaddrinfo error:%s",gai_strerror(err));
        exit(1);
    } 
    for(aip=alist;aip!=NULL;aip=aip->ai_next)
    {
        if((sockfd=initsever(SOCK_STREAM,aip->ai_addr,aip->ai_addrlen,QLEN))>=0)
        {
            sever(sockfd);
            exit(0);
        }
    }
    exit(1);
}
