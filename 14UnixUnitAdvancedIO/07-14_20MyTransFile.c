/*************************************************************************
	> File Name: 07-14_20MyTransFile.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Mon 01 Feb 2016 01:17:13 AM CST
 ************************************************************************/

#include "apue.h" 
#include<ctype.h>
#include<fcntl.h>

#define BSZ 4096
unsigned char buf[BSZ];

unsigned char translate (unsigned char c )
{
    if(isalpha(c))
    {
        if(c>='n')
            c-=13;
        else if(c>='a')
            c+=13;
        else if(c>='N')
            c-=13;
        else 
            c+=13;
    }
    return c;
}

int  main(int argc ,char ** argv)
{
    int ifd,ofd, i,n ,nw;
    if(argc!=3)
        err_quit("usage:rotl3 infile  outfile ");
    if((ifd=open(argv[1],O_RDONLY))<0)
        err_sys("open file  %s error", argv[1]);
    if((ofd=open(argv[2],O_RDWR|O_CREAT|O_TRUNC,FILE_MODE))<0)
        err_sys("can't create  file %s",argv[2]);

    while((n=read(ifd,buf,BSZ))>0)
    {
        for(i=0;i<n;i++)
        {
            buf[i]=translate(buf[i]);
        }
        if((nw=write(ofd,buf,n))!=n)
        {
            if(nw<0)
                err_sys("write file wrong");
            else
                err_quit("short write (%d/%d)",nw,n);
        }
    }
    fsync(ofd);
    exit(0);
}

