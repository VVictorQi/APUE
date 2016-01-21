/*************************************************************************
	> File Name: 7-10.11MySignalSet.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Thu 21 Jan 2016 05:54:25 PM CST
 ************************************************************************/
#include"apue.h"
#include<stdio.h>
#include<signal.h>
#include<errno.h>

#define SIGBAD(signo)((signo)<=0||(signo)>=NSIG)
/*int sigaddset(sigset_t *set, int signo)
{
    if(SIGBAD(signo))
    {
        errno=EINVAL;
        return -1;
    }
    *set |= 1<<(signo -1); //将1转换为bit位二进制,然后对信号进行按位或运算将其位其位置为1
    return 0;
}

int sigdelset(sigset_t *set, int signo)
{
    if(SIGBAD(signo))
    {
        errno=EINVAL;
        return -1;
    }
     *set &=(~(1<<(signo-1)));//同理将其位置为0
    return 0;
}//这三个函数会出现同样的错误就是类型不匹配的问题 sigset_t 和int型
int sigismember(const sigset_t *set , int signo)
{
    if(SIGBAD(signo))
    {
        errno=EINVAL;
        return -1;
    }
    return ((*set &(1<<(signo-1)))!=0);
}
//*///由于内部定义了这三个函数，所以在此注释掉，要不然会造成重定义

//利用pr_mask 函数 sigprocmask函数 设置信号阻塞

void pr_mask(const char *str)
{
    sigset_t sigset;
    int errno_save;
    errno_save=errno;
    if(sigprocmask(0,NULL,&sigset))
    {
        err_ret("sigprocmask error");
    }
    else
    {
        printf("%s",str);
        if(sigismember(&sigset,SIGINT))
            printf("SIGINT");
        if(sigismember(&sigset,SIGQUIT))
            printf("QUIT");
        if(sigismember(&sigset,SIGUSR1))
            printf("USR1");
        if(sigismember(&sigset,SIGUSR2))
            printf("USR2");
        printf("\n");
    }
    errno =errno_save;
}
static void sig_quit(int signo)
{
    printf("caught SIGQUIT\n");
    if(signal(SIGQUIT,SIG_DFL)==SIG_ERR)
        err_sys("can't reset SIGQUIT");
}
int main(void)
{
    sigset_t newmask,oldmask,pendmask;
    if(signal(SIGQUIT,sig_quit)==SIG_ERR)
        err_sys("can't catch SISGQUIT");

   
    sigemptyset(&newmask);
    sigaddset(&newmask,SIGQUIT);
    if(sigprocmask(SIG_BLOCK,&newmask,&oldmask)<0)
        err_sys("SIG_BLOCK errror");
    sleep(5);
    if(sigpending(&pendmask)<0)
        err_sys("sigpending error");
    if(sigismember(&pendmask,SIGQUIT))
        printf("\n SIGQUIT pending\n");


    if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0)
        err_sys("SIG_SETMASK error");
    printf("SIGQUIT unblocked \n");
    sleep(10);
    exit(0);
    
}


















