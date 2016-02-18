/*************************************************************************
	> File Name: 07-15_15MyExternPopenGetFileToExec.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Fri 19 Feb 2016 12:00:29 AM CST
 ************************************************************************/

#include "apue.h" 
#include<sys/wait.h>


int main(void)
{
    char        line[MAXLINE];
    FILE        *fpin;
    if((fpin=popen("./06-15_14MySimpleCp","r"))==NULL)
        err_sys("popen  error");
    for(;;)
    {
        fputs("promat >",stdout);
        fflush(stdout);


        if(fgets(line,MAXLINE,fpin)==NULL)//read from pipe
            break;
        if(fputs(line,stdout)==-1)
            err_sys("fputs error to pipe");
    }
    if(pclose(fpin)==-1)
        err_sys("pcloselose error");
    putchar('\n');
    exit(0);
}
