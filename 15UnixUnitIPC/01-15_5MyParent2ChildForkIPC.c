/*************************************************************************
	> File Name: 01-15_5MyParent2ChildForkIPC.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Wed 17 Feb 2016 11:15:26 AM CST
 ************************************************************************/

#include "apue.h" 
int main()
{
    int     n;
    int     fd[2];
    pid_t   pid;
    char    line[MAXLINE];
    
    if(pipe(fd)<0)
        err_sys("pipe error");
    if((pid=fork())<0)
        err_sys("fork error");
    else if(pid==0)
    {
        close(fd[1]);
        n=read(fd[0],line,MAXLINE);
        write(STDOUT_FILENO,line,n);
    }
    else 
    {
        close(fd[0]);
        write(fd[1],"hello world \n",12);
    }
    exit(0);
}
