/*************************************************************************
	> File Name: 02-13_6MySigleDaemon.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Thu 28 Jan 2016 09:37:14 PM CST
 ************************************************************************/
#include"apue.h"
#include<syslog.h>
#include<pthread.h>


sigset_t mask;

#define LOCKFILE   "/var/run/daemon.pid"
#define LOCKMODE   (S_IRUSR|S_IWUSR|S_IROTH)


extern int lockfile(int);


int already_running(void)
{
    int ffd;
    char buf[16];
    fd=open(LOCKFILE,O_RDWR|O_CREAT|LOCKMODE);
    if(fd<0)
    {
        syslog(LOG_ERR,"can;t open %s: %s",LOCKFILE,stderror(errno));
        exit(1);
    }
    if(lockfile(fd)<0)
    {
        if(errno==EACCES||errno==EAGAIN)
        {
            close(fd);
            return 1;
        }
        syslog(LOG_ERR,"can't lock %s:%s",LOCKFILE,stderror(errno));
        exit(1);
    }
    ftruncate(fd,0);
    sprintf(buf,"%ld",(long)getpid());
    write(fd,buf,strlen(buf)+1);
    return 0;
}
void reread(void)
{

}

void *thr_fn(void *arg)
{
    int err,signo;
    for(;;)
    {
        err=sigwait(&mask,&signo);
        if(err!=0)
        {
            syslog(KOG_ERR,"sigwait failed ");
            exit(1);
        }
        switch(signo)
        {
            case SIGHUP:
                syslog(LOG_INFO,"Re-reading configuration  file");
                reread();
                break;
            case SIGTERM:
                syslog(LOG_INFO,"Got SIGTERM; exiting");
                exit(0);
            default:
                syslog(LOG_INFO,"unexpected signal %d \n",signo);
        }
    }
    return 0;
}

int main(int argc,cahr ** argv)
{
    int err;
    pthread_t  tid;
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
    sa.sa_handler=SIG_DEL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags=0;
    if(sigaction(SIGHUP,&sa,NULL)<0)
        err_quit("%s:can't restore SIGHUP  default");
    sigfillset(&mask);
    if((err=pthread_sigmask(SIG_BLOCK,&mask,NULL))!=0)
        err_exit(err,"SIG_BLOCK error");
    err=pthread_create(&tid,NULL,thr_fn,0);
    if(err!=0)
        err_exit(err,"can't create the thread");
    exit(0);

}








