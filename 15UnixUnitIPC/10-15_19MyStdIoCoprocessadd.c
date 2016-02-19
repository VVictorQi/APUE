/*************************************************************************
	> File Name: 10-15_19MyStdIoCoprocessadd.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Fri 19 Feb 2016 11:38:53 PM CST
 ************************************************************************/

#include "apue.h" 

int main(void)
{
    int n, numone,numtwo;
    char line[MAXLINE];


    while(fgets(line,MAXLINE,stdin)!=NULL)
    {
        if(sscanf(line,"%d%d",&numone,&numtwo)==2)
        {
            if(printf("%d\n",numone+numtwo)==EOF)
                err_sys("printf error");
        }
        else
        {
           if( printf("invalid args ")==EOF)
                err_sys("printf error");
        }
    }
    exit(0);
}
