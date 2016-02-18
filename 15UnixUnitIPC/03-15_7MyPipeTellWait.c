/*************************************************************************
	> File Name: 03-15_7MyPipeTellWait.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Thu 18 Feb 2016 08:51:15 PM CST
 ************************************************************************/

#include "apue.h" 

static int pfd1[2],pfd2[2];

//利用pipe将其重写，不同于上次的信号版本，本次版本主要用于进程间通信
void TELL_WAIT(void)
{
    if(pipe(pfd1)<0||pipe(pfd2)<0)
        err_sys("pipe error");
}

void TELL_PARENT(pid_t pid)
{
    if(write(pfd2[1],"c",1)!=1)
        err_sys("write error");
}
void WAIT_PARENT(void)
{
    char c;
    if(read(pfd1[0],&c,1)!=1)
        err_sys(read error);
    if(c!='p')
        err_quit("WAIT_PARENT:incorrect data");
}

void WAIT_CHILD(pid_t pid)
{
    if(write(pfd1[1],"p",1)!=1)
        err_sys("write error");
}
void WAIT_CHILD(void)
{
    char c;
    if(read(pfd2[0],&c,1)!=1)
        err_sys("read error");
    if(c!="c")
        err_quit("WAIT_CHILD:incorrect data");
}
void TELL_WAIT(void)
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
