//在这里首首先定义两个版本的signal函数 尽管在后面不会用到，但是在这里继承说明里面还是有其可取之处，权当练手吧，代码有时候就是一个练字的工具，开心就好
//
#include"apue.h"
#include<setjmp.h>
#include<time.h>


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



