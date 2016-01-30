/*************************************************************************
	> File Name: 04-14_7MyDiedLock.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Sat 30 Jan 2016 03:09:56 PM CST
 ************************************************************************/

#include "apue.h" 
#include<fcntl.h>
#include"0214_5MyAccReleaseLock.c"
static void lockabyte(const char * name ,int fd, off_t offset)
{
    if(writew_lock(fd,offset,SEEK_SET,1)<0)
        err_sys("%s: writew_lock  error ",name);
    printf("%s: got the lock ,byte %lld \n ",name , ( long long )offset);
}
int main (void)
{
    int fd;
    pid_t pid;


    if((fd=creat("templock",FILE_MODE))<0)
        err_sys("create file error");
    if(write(fd,"ab",2)!=2)
        err_sys("write errpr");


    TELL_WAIT();
    if((pid=fork())<0)
    {
        err_sys("fork() error");
    }
    else if (pid==0)
    {
        lockabyte("child",fd,0);
        TELL_PARENT(getpid());
        WAIT_PARENT();
        lockabyte("child",fd,1);
    }
    else 
    {
        lockabyte("parent", fd,1);
        TELL_CHILD(pid);
        WAIT_CHILD();
        lockabyte("parent", fd,0);
    }
    exit(0);
}
