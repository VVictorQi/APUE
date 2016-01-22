//在这里首首先定义两个版本的signal函数 尽管在后面不会用到，但是在这里继承说明里面还是有其可取之处，权当练手吧，代码有时候就是一个练字的工具，开心就好
//
#include"apue.h"
#include<setjmp.h>
#include<time.h>

//关于setjmp系列函数就是说在信号处理程序中使用了siglongjmp函数就应该使用过一种保护机制。就是仅在调用sigsetjmp之后将变量canjump置非零值。在信号处理程序中检测此变量，当他非零时才调用siglongjmp函数返回main主函数，这提供了一种保护机制，让跳转缓冲jmpbuf在sigsetjmp尚未初始化的时候，防止调用信号处理函数，但是在一般的c代码中并不需要这种保护机制，但是信号不行，因为信号随时有可能发生
Sigfunc* signal (int signo,Sigfunc*func)
{
    struct sigaction act,oact;
    act.sa_handler=func;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    if(signo==SIGALRM)
    {
        #ifdef  SA_INTERRUPT
            act.sa_flags|SA_INTERRUPT;
        #endif
    }
    else 
    {
        act.sa_flags|=SA_RESTART;
    }
    if(sigaction(signo, &act,&oact)<0)
        return (SIG_ERR);
    return oact.sa_handler;
}
//注意必须用sigemptyset函数初始化act结构的sa_mask成员，不能保证act.sa_mask=0会做同样的事情
//咋以下一个版本取消了对SA_RESTART的处理，阻止了被中断的系统调用重启动
Sigfunc* signal_intr(int signo,Sigfunc*func)
{
    struct sigaction act,oact;
    act.sa_handler=func;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;    
    #ifdef  SA_INTERRUPT
         act.sa_flags|SA_INTERRUPT;
    #endif
    if(sigaction(signo, &act,&oact)<0)
        return (SIG_ERR);
    return oact.sa_handler;
}
static void                  sig_usr1(int);
static void                  sig_alrm(int);
static sigjmp_buf            jmpbuf;
static volatile sig_atomic_t canjump;
int main(void)
{
    if(signal(SIGUSR1,sig_usr1)==SIG_ERR)
        err_sys("singnal SIGUSR1 error");
    if(signal(SIGALRM,sig_alrm)==SIG_ERR)
        err_sys("signal SIGALRM error");
    pr_mask("staring main : \n");
    if(sigsetjmp(jmpbuf,1))
    {
        pr_mask("ending main: \n");
        exit(0);
    }
    canjump=1;
    for(;;)
    {
        pause(); //终止保持信号接收
    }
}

static void sig_usr1(int signo)
{
    time_t startime;
    if(canjump==0)
    {
        return ;
    }
    pr_mask("staring sig_usr1 function\n");
    alarm (3);
    startime=time(NULL);
    for(;;)
    {
        if(time(NULL)>startime +5)
            break;
    }
    pr_mask("finishing the function sig_usr1\n");
    canjump =0;
    siglongjmp(jmpbuf,1);//返回主函数同前一例中的longjmp作用类似

}
static void sig_alrm(int signo )
{
    pr_mask("int the function sig_alrm\n");
}



