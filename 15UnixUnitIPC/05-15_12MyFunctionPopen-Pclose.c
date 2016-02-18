/*************************************************************************
	> File Name: 05-15_12MyFunctionPopen-Pclose.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Thu 18 Feb 2016 10:26:07 PM CST
 ************************************************************************/

#include "apue.h" 
#include<errno.h>
#include<fctnl.h>
#include<sys/wait.h>
#include<limits.h>//针对于第二章出现的open_max函数，直接将此函数集成在这里

static pid_t *childpid=NULL;//now we set a poiter of pid_t
static int maxfd;
#ifdef OPEN_MAX
static long openmax=OPEN_MAX;
#else static long openmax=0;
#endif




#define OPEN_MAX_GUESS 256

long open_max(void)
{
    if(openmax==0)
        errno=0;
    if((openmax=sysconf(_SC_OPEN_MAX))<0)//get configuration information at run Time
    {
        if(errno==0)
            openmax=OPEN_MAX_GUESS;
        else
            err_sys("sysconf error for  _SC_OPEN_MAX");
    }
    return (openmax)
}

FILE*popen(const char *cmdstring,const char *type)
{
    int                     i;
    int                     pfd[2];
    pid_t                   pid;
    FILE                    *fp;
    
    // for type just allow "w" or "r" mode

    if((type[0])!='r'&&type[0]!='w'||type[1]!=0)
    {
        errno=EINVAL;
        return (NULL);
    }//限制type参数属性
    if(childpid==NULL)
    {
        maxfd=open_max();
        if((childpid=calloc(maxfd,sizeof(pid_t)))==NULL)
            return (NULL);
    }

    if(pipe(fpd)<0)
        return (NULL);   //设置管道
    if(pfd[0]>=maxfd||pfd[1]>=maxfd)
    {
        close(pfd[0]);
        close(pfd[1]);
        errno=EMFILE; //EMFILE          Too many open files man手册的解释  
        return (NULL);
    }
    if((pid=fork())<0)
    {
        err_sys("fork error");
        return(NULL);
    }
    /*
    *makes newfd be the copy of oldfd, closing newfd first if necessary, but notethe following:
    * If oldfd is not a valid file descriptor, then the call fails, and  newfd  is  not  closed.
    *If  oldfd is a valid file descriptor, and newfd has the same value as oldfd, then dup2() does nothing, and returns newfd.
    *After a successful return from one of these system  calls,  the  old  and  new  file
    *descriptors  may be used interchangeably.  They refer to the same open file description (see open(2)) 
    *and thus share file offset and file status flags; for example, if the  file offset is modified 
    *by using lseek(2) on one of the descriptors, the offset is also changed for the other.
    *The two descriptors do not share file descriptor  flags  (the  close-on-exec  flag).
    *The  close-on-exec  flag  (FD_CLOEXEC; see fcntl(2)) for the duplicate descriptor is off.
    */
    else if(pid==0)
    {
        if(*type=='r')
        {
            close(pfd[0]);
            if(pfd[1]!=STDOUT_FILENO)
            {
                dup2(pfd[1],STDOUT_FILENO);
                close(pfd[1]);
            }
        }
        else
        {
            close(pfd[1]);
            if(pfd[0]!=STDIN_FILENO)
            {
                dup2(pfd[0],STDIN_FILENO);
                close(pfd[0]);
            }
        }
        for(i=0;i<maxfd;i++)
            if(childpid[i]>0)
                close[i];
            execl("/bin/sh","sh","-c",cmdstring,(char *)0);
            _exit(127);
    }
   //对于父进程继续 
   if(*type=='r')
   {
       close(pfd[1]);
       if((fp=fdopen(pfd[0],type))==NULL)
            return(NULL);
   }
   else
   {
       close(pfd[0]);
       if((fp=fdopen(pfd[1],type))==NULL)
            return(NULL);
   }
   childpid[fileno(fp)]=pid;//记录子进程pid
   return(fp);
}

int pclose(FILE*fp)
{
    int         fd,stat;
    pid_t       pid;



    if(childpid==NULL)
    {
        errno=EINVAL;
        return (-1);
    }

    fd=fileno(fp);
    if(fd>=maxfd)
    {
        errno=EINVAL;
        return (-1);
    }
    if((pid=childpid[fd]==0))
    {
        errno=EINVAL;
        return(-1);
    }

    childpid[fd]=0;
    if(fclose(fp)==EOF)
        return(-1);
    while(waitpid(pid,&stat,0)<0)
    {
        if(errno!=EINTR)
            return (-1);
    }

    return (stat);//返回文件状态
}


