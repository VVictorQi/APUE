/*************************************************************************
	> File Name: 6-10MyalarmLongjmp.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Thu 21 Jan 2016 05:21:55 PM CST
 ************************************************************************/

#include<stdio.h>
#include"apue.h"
#include<setjmp.h>
static void  sig_alrm(int);
static jmp_buf env_alrm;
int main(void)
{
    int n;
    char line[MAXLINE];
    if(signal(SIGALRM,sig_alrm)==SIG_ERR)
        err_sys("signal error");
    if(setjmp(env_alrm)!=0)
        err_quit("read timeout\n");
    alarm(10);
    if((n=read(STDIN_FILENO,line,MAXLINE))<0)
        err_sys("read error");
    alarm(0);
    write(STDOUT_FILENO,line,n);
    exit(0);
}
static void sig_alrm(int signo)
{
    longjmp(env_alrm,1);
}
