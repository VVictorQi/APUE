/*************************************************************************
	> File Name: 04-15_6MyPopenOveriedCp.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Thu 18 Feb 2016 09:51:19 PM CST
 ************************************************************************/

#include "apue.h" 
#include<sys/wait.h>

#define     PAGER   "${PAGER:-more}" //环境变量
//宏定义的意思是，如果shell变量PAGER已经定义，切其值非空，就使用这个值，否则使用字符串more
int main(int argc,char **argv)
{
    char line[MAXLINE];
    FILE*fpin,*fpout;

    if(argc!=2)
        err_quit("usage:%s<pathname>",argv[0]);
    if((fpin=fopen(argv[1],"r"))==NULL)
        err_sys("can't open %s",argv[1]);
    if((fpout=popen(PAGER,"w"))==NULL)
        err_sys("powpen error");
    
    while(fgets(line,MAXLINE,fpin)!=NULL)
    {
        if(fputs(line,fpout)==EOF)
            err_sys("fput error to pipe");
    }
    if(ferror(fpin))
        err_sys("fgets error");
    if(pclose(fpout)==-1)
        err_sys("pclose error");

    exit(0);
}
