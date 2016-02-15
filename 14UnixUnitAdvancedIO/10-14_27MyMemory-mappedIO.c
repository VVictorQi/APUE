/*************************************************************************
	> File Name: 10-14_27MyMemory-mappedIO.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Mon 15 Feb 2016 11:06:22 PM CST
 ************************************************************************/

#include "apue.h" 
#include<fcntl.h>
#include<sys/mman.h>

#define COPYINCR   (1024*1024*1024) //表示1GB容量
int main(int argc,char ** argv)
{
    int         fdin,fdout;
    void        *src,*dst;
    size_t      copysz;
    struct stat sbuf;
    off_t       fsz=0;


    if(argc!=3)
        err_quit("usage: %s<fromfile><tofile>",argv[0]);
    if((fdin=open(argv[1],O_RDONLY))<0)
        err_sys("can't open %s for reading ",argv[1]);
    if((fdout=open(argv[2],O_RDWR|O_CREAT|O_TRUNC,FILE_MODE))<0)
        err_sys("can't creat file of %s for writing ",argv[2]);

    if(fstat(fdin,&sbuf)<0)
        err_sys("fstat error");
    if(ftruncate(fdout,sbuf.st_size)<0)
        err_sys("ftruncate error");

    while (fsz<sbuf.st_size)
    {
        if((sbuf.st_size-fsz)>COPYINCR)
            copysz=COPYINCR;
        else
            copysz=sbuf.st_size-fsz;
        if((src=mmap(0,copysz,PROT_READ,MAP_SHARED,fdin,fsz))==MAP_FAILED)
            err_sys("mmap error for input ");
        if((dst=mmap(0,copysz,PROT_READ|PROT_WRITE,MAP_SHARED,fdout,fsz))==MAP_FAILED)
            err_sys("mmap error for output");

        memcpy(dst,src,copysz);
        munmap(src,copysz);
        munmap(dst,copysz);
        fsz+=copysz;
    }
    exit(0);
}
//采用mmap存储映射io重写的一个类似于copy命令的代码段。增加了用户时间，
//减少了系统调用的时间，在某些场合还是很方便的
