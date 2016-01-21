/*************************************************************************
	> File Name: 5-10_10Myalarm.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Thu 21 Jan 2016 04:45:07 PM CST
 ************************************************************************/

#include<stdio.h>
#include"apue.h"
static void sig_alrm(int signo)
{
//    longjmp(env_alrm,1);
}
int main(void)
{
    int n;
    char line[MAXLINE];
    if(signal(SIGALRM,sig_alrm)==SIG_ERR)
        err_sys("signal error");
    alarm(10);
    if((n=read(STDIN_FILENO,line,MAXLINE))<0)
        err_sys("read error");
    alarm(0);
    write(STDOUT_FILENO,line,n);
    exit(0);

}

