/*************************************************************************
	> File Name: 07-16_MyExternSever.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Fri 26 Feb 2016 10:42:28 PM CST
 ************************************************************************/

#include "apue.h" 
#include<netdb.h>
#include<errno.h>
#include<syslog.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<./04-16_12MyInitSocketFunction.c>
#define QLEN        10
#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif



void  sever(int sockfd)
{
    int             clfd,status;
    pid_t           pid;

    set_cloexec(sockfd);//设置关闭位
    for(;;)
    {
        if((clfd=accept(sockfd,NULL,NULL))<0)
        {
            syslog(LOG_ERR,"ruptime:accept error:%s",strerror(errno));//打上错误日志
            exit(1);
        }
        if((pid=fork())<0)
        {
            syslog(LOG_ERR,"ruptime: fork  error:%s",strerror(errno));//打上错误日志
            exit(1);
        }
        else if
    }
}




