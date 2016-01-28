/*************************************************************************
	> File Name: 01-13_1MyinitDaemonFork.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Thu 28 Jan 2016 05:46:10 PM CST
 ************************************************************************/

#include "apue.h" 
#include<syslog.h>
#include<fcntl.h>
#include<sys/resource.h>
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


void main(void)
{
    printf("staring  daemon ");
    daemonize("ls");
    for(;;) 
        sleep(10);
}
