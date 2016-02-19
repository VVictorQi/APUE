/*************************************************************************
	> File Name: 08-15_17MyCoprocess.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Fri 19 Feb 2016 10:26:04 PM CST
 ************************************************************************/

#include "apue.h" 

int main(void)
{
    int n, numone,numtwo;
    char line[MAXLINE];


    while((n=read(STDIN_FILENO,line,MAXLINE))>0)
    {
        line[n]=0;
        if(sscanf(line,"%d%d",&numone,&numtwo)==2)
        {
            sprintf(line,"%d\n",numone+numtwo);
            n=strlen(line);
            if(write(STDOUT_FILENO,line,n)!=n)
                err_sys("write error");
        }
        else
        {
           if( write(STDOUT_FILENO,"invalid args\n",13)!=13)
                err_sys("write error");
        }
    }
    exit(0);
}
