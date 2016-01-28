/*************************************************************************
	> File Name: 0_8MyPthreadGetSignalConf.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Fri 29 Jan 2016 12:32:20 AM CST
 ************************************************************************/
#include"apue.h"
#include<syslog.h>
#include<pthread.h>
#include<sys/resource.h>
#include<fcntl.h>
#include<stdio.h>
#include<errno.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<string.h>
sigset_t mask;
#define LOCKFILE   "/var/run/daemon.pid"
#define LOCKMODE   (S_IRUSR|S_IWUSR|S_IROTH)

//不知道其他人有没有遇到我这种情况 lockfile函数无法打开，然后man了一下发现并没有lockfile 
//这个函数，但是在网上查找的资源是有lockfile函数的 ，系统是centos ，可能有关于系统差异吧
//虽然都符合poxis标准，在unbutu下测试也没有，lockfile函数应该是作者实现的函数，或者如今被标准废弃
//但是显然书籍版本比较新所以函数应该存在。
void sigterm(int signo)
{
    syslog(LOG_INFO,"got sigterm ;exiting");
    exit(0);
}

void sighup(int signo)
{
    syslog(LOG_INFO,"RE-reading configuretiiong file ");
    reread();
}
int lockfile(int fd)
{                   
    struct flock fl;
    fl.l_type=F_WRLCK;
    fl.l_start=0;
    fl.l_whence=SEEK_SET;
    fl.l_len=0;
    return fcntl(fd,F_SETLK,&fl);
}


void daemonize(const char *cmd)
{
    int                 i,fd0,fd1,fd2;
    pid_t               pid;
    struct              rlimit rl;
    struct sigaction    sa;


    umask(0);

    if(getrlimit(RLIMIT_NOFILE,&rl)<0)
        err_quit("%s: can't get file limit ",cmd);

    //first blood :fork 一次，使父进程退出，让子进程成为孤儿进程，让子进程成为新会话的手进程，
 
    if ((pid=fork())<0)
        err_quit("%s:can't fork ",cmd);
    else if (pid!=0)
        exit(0);
    setsid();

    sa.sa_handler=SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags=0;

    if(sigaction(SIGHUP,&sa,NULL)<0)
        err_quit("%s: can't ignore SIGHUP ",cmd);
    if((pid=fork())<0)
        err_quit("%s: fork error ",cmd);
    else if(pid!=0)
        exit(0);




    if(chdir("/")<0)
        err_quit("%s: can;t cahnge directory to /",cmd);




    if(rl.rlim_max==RLIM_INFINITY)
        rl.rlim_max=1024;
    for(i=0;i<rl.rlim_max;i++)
        close(i);

    fd0=open("/dev/null",O_RDWR);
    fd1=dup(0);
    fd2=dup(0);




    openlog(cmd,LOG_CONS,LOG_DAEMON);
    if(fd0!=0||fd1!=1||fd2!=2)
    {
        syslog(LOG_ERR,"unexpected file decription %d %d  %d",fd0,fd1,fd2);
        exit(1);
    }
}
int already_running(void)
{
    int fd;
    char buf[16];
    fd=open(LOCKFILE,O_RDWR|O_CREAT|LOCKMODE);
    if(fd<0)
    {
        syslog(LOG_ERR,"can;t open %s: %s",LOCKFILE,strerror(errno));
        exit(1);
    }
    if(lockfile(fd)<0)
    {
        if(errno==EACCES||errno==EAGAIN)
        {
            close(fd);
            return 1;
        }
        syslog(LOG_ERR,"can't lock %s:%s",LOCKFILE,strerror(errno));
        exit(1);
    }
    ftruncate(fd,0);
    sprintf(buf,"%ld",(long)getpid());
    write(fd,buf,strlen(buf)+1);
    return 0;
}
void reread(void)
{
    printf("我必须得输出些东西来表明我确实被用过了");
}


int main(int argc,char ** argv)
{
    char *cmd;
    struct sigaction sa;




    if((cmd=strrchr(argv[0],'/'))==NULL)
        cmd=argv[0];
    else
        cmd++;
    daemonize(cmd);




    if(already_running())
    {
        syslog(LOG_ERR,"daemon  already running");
        exit(1);
    }
    sa.sa_handler=sigterm;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask,SIGHUP);
    sa.sa_flags=0;
    if(sigaction(SIGTERM,&sa,NULL)<0)
    {
        syslog(LOG_ERR,"can't catch SIGTERM:%s",strerror(errno));
        exit(0);
    }
    sa.sa_handler=sighup;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask,SIGTERM);
    sa.sa_flags=0;
    if(sigaction(SIGHUP,&sa,NULL)<0)
    {
        syslog(LOG_ERR,"can't catch SIGHUP:%s",strerror(errno));
        exit(0);
    }
    exit(0);
}

