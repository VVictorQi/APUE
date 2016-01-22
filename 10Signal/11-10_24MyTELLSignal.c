/*************************************************************************
	> File Name: 11-10_24MyTELLSignal.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Fri 22 Jan 2016 03:50:07 PM CST
 ************************************************************************/

#include "apue.h" 

//主要讲8.9节中说到的五个例程的实现
//分别是TELLWAIT、TELL_PARENT、TELL_CHILD、WAIT_PARENT、WAIT_CHILD


static volatile sig_atomic_t sigflag;
static sigset_t newmask,oldmask,zeromask;

static void sig_usr(int signo)
{
    sigflag=1;
}
void TELLWAIT(void)
{
    if(signal(SIGUSR1,sig_usr)==SIG_ERR)
        err_sys("signal sigusr1 error");
    if(signal(SIGUSR2,sig_usr)==SIG_ERR)
        err_sys("signal sigusr2 error");
    sigemptyset(&newmask);
    sigemptyset(&zeromask);
    sigaddset(&newmask,SIGUSR1);
    sigaddset(&newmask,SIGUSR2);

    if(sigprocmask(SIG_BLOCK,&newmask,&oldmask)<0)
        err_sys("SIG_BLOCK error");
}

void TELL_PARENT(pid_t pid)
{
    kill(pid,SIGUSR2);
}
void WAIT_PARENT(void)
{
    while(sigflag==0)
        sigsuspend(&zeromask);
    sigflag=0;
    if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0)
        err_sys("SIG_SETMASK error");
}
void TELL_CHILD(pid_t pid)
{
    kill(pid,SIGUSR1);
}
void WAIT_CHILD(void)
{

    while(sigflag==0)
        sigsuspend(&zeromask);
    sigflag=0;
    if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0)
        err_sys("SIG_SETMASK error");
}
}
