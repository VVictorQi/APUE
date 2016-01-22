/*************************************************************************
	> File Name: 9-10-22MySigsuspend.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Fri 22 Jan 2016 01:51:03 PM CST
 ************************************************************************/

//先来一段不成熟的代码<code>
/*
    sigset_t  newmask,oldmask;
    sigemptyset(&newmask);
    sigaddset(&newmask,SIGINT);
    if(sigprocmask(SIG_BLOCK,&newmask,&oldmask)<0)
        err_sys("sigprocmask error");
    if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0)
        err_sys("SIG_SETMASK error");
    pause();



        这个问题是在信号阻塞时，产生了信号，那么该信号就会被推迟到他解除阻塞， 对应用程序而言
    该信号好像发生在解除对SIGINT的阻塞和pause之间，当然这取决于内核的实现方法。
        如果发生了这种情况，或者如果在解除阻塞时刻和pause之间确实发生了信号，那么就会产生问题
    因为可能不会再见到该信号，所以从这种意义上讲，在此时间窗口中发生的信号丢失了 ，  这样就让
    pause被永远阻塞。这是早期的不可靠信号的另一个问题。
        为纠正此问题，需要在一个原子操作中先回复信号屏蔽字，然后是进程休眠。这种功能就是下面的
    sigsuspend函数提供的
*/
#include<stdio.h>
#include"apue.h"


static void sig_int(int signo )
{
    pr_mask("'n function in sig_int:");
}
int  main(void)
{
    sigset_t newmask,oldmask,waitmask;
    pr_mask("program start:");
    if(signal(SIGINT,sig_int)==SIG_ERR)
        err_sys("signal SIGINT error");
    sigemptyset(&waitmask);
    sigaddset(&waitmask,SIGUSR1);
    sigemptyset(&newmask);
    sigaddset(&newmask,SIGINT);
    if(sigprocmask(SIG_BLOCK,&newmask,&oldmask))
        err_sys("SIG_BLOCK error");
    pr_mask("in the critical region:");

    if(sigsuspend(&waitmask)!=-1)
        err_sys("sigsuspen error");
    pr_mask("after return from sigsuspend");
    if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0)
        err_sys("SIG_SETMASK errror");
    pr_mask("program exit: ");
    exit(0);

}
