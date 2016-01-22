/*************************************************************************
	> File Name: 10-10_23MySigsuspendWait.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Fri 22 Jan 2016 02:44:36 PM CST
 ************************************************************************/

#include "apue.h" 
volatile sig_atomic_t quitflag;
static void sig_int(int signo)
{
    if(signo==SIGINT)
        printf("\n interrupt SIGINT\n");
    else  if(signo == SIGQUIT)
    {
        quitflag=1;
        printf("\n interrupt SIGQUIT\n");
    }
}
int main(void)
{
    sigset_t newmask,oldmask,zeromask;
    if(signal(SIGINT,sig_int)==SIG_ERR)
        err_sys("signal sigint error");
    if(signal(SIGQUIT,sig_int)==SIG_ERR)
        err_sys("signal sigquit error");

    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask,SIGQUIT);


    //sigsuspend 的另一种应用是等待一个信号处理程序设置一个全局变量，
    //就是当希望捕捉某信号时，才唤醒主例程。在此往往通过这个全局变量来决定

    if(sigprocmask(SIG_BLOCK,&newmask,&oldmask)<0)
        err_sys("SIG_BLOCK error");
    while(quitflag==0)
    {
        sigsuspend(&zeromask);
    }
    quitflag=0;
    //SIG_SETMASK该进程新的信号屏蔽是set指向的值
    if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0)
        err_sys("SIG_SETMASK error");
    exit(0);
}
