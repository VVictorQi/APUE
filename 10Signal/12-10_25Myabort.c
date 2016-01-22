/*************************************************************************
	> File Name: 12-10_25Myabort.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Fri 22 Jan 2016 04:29:29 PM CST
 ************************************************************************/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
void abort(void)
{
    sigset_t mask;
    sigaction action;

    sigaction(SIGABBR,NULL,&action);
    if(action.sa_handler== SIG_IGN)
    {
        action.sa_handler=SIG_IGN;
        sigaction(SIGADRT,&action,NULL);
    }
    if(action.sa_handler==SIG_DFL)
        fflush(NULL);//刷新缓冲区
    sigfillset(&mask);
    sigdelset(&mask,SIGABRT);//只将SIGABRT屏蔽字关闭
    sigprocmask(SIG_SETMASK,&mask,NULL);
    kill(getpid(),SIGABRT);



    fflush(NULL);
    action.sa_handler=SIG_DFL;
    sigaction(SIGABRT,&action,NULL);
    sigprocmask(SIG_SETMASK,&mask,NULL);
    kill(getpid(),SIGABRT);
    exit(1);
}
//首先查看是否执行默认动作，若是则冲洗标准io流，这并不等价于对所有打开的流调用fclose 因为只冲洗，并不关闭
//但是当进程终止时，系统会关闭所有打开的文件。如果进程捕捉此信号并返回，那么因为进程可能产生了更多的输出，
//所以再一次冲洗所有流。不进行冲洗处理的唯一条件是如果进程捕获该信号，然后调用_exit 或_Exit.
//在这种情况下，任何尚未冲洗的内存中的标准io缓冲都被废弃。假设我们捕捉此信号。而且_Exit或_exit的调用者并不想要冲洗缓冲区。
//我们阻塞了除SIGABRT外所有的信号，这样就可知如果对kill的调用返回了，则该进程一定已经捕捉到该信号，并且也已经从该信号处理程序中返回
