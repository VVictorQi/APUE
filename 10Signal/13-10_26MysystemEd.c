/*************************************************************************
	> File Name: 13-10_26MysystemEd.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Fri 22 Jan 2016 05:05:13 PM CST
 ************************************************************************/

#include "apue.h" 
static void sig_int(int signo )
{
    printf("caugth SIGINT \n");
}
static void sig_chld(int signo )
{
    printf("caugth SIGCHLD\n");
}
int main(void)
{
    if(signal(SIGINT,sig_int)==SIG_ERR)
        err_sys("signal sigint error");
    if(signal(SIGCHLD,sig_chld)==SIG_ERR)
        err_sys("signal sigchld error");

    if(system("/bin/ed")<0)
        err_sys("system() error");
    exit (0);
}
