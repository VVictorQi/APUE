#include "apue.h"
#include <errno.h>		/* for definition of errno */
#include <stdarg.h>		/* ISO C variable aruments */
#include<fcntl.h>
static void	err_doit(int, int, const char *, va_list);
/*
 * Nonfatal error related to a system call.
 * Print a message and return.
 */
void
err_ret(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
}

/*
 * Fatal error related to a system call.
 * Print a message and terminate.
 */
void
err_sys(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
	exit(1);
}

/*
 * Nonfatal error unrelated to a system call.
 * Error code passed as explict parameter.
 * Print a message and return.
 */
void
err_cont(int error, const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, error, fmt, ap);
	va_end(ap);
}

/*
 * Fatal error unrelated to a system call.
 * Error code passed as explict parameter.
 * Print a message and terminate.
 */
void
err_exit(int error, const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, error, fmt, ap);
	va_end(ap);
	exit(1);
}

/*
 * Fatal error related to a system call.
 * Print a message, dump core, and terminate.
 */
void
err_dump(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(1, errno, fmt, ap);
	va_end(ap);
	abort();		/* dump core and terminate */
	exit(1);		/* shouldn't get here */
}

/*
 * Nonfatal error unrelated to a system call.
 * Print a message and return.
 */
void
err_msg(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(0, 0, fmt, ap);
	va_end(ap);
}

/*
 * Fatal error unrelated to a system call.
 * Print a message and terminate.
 */
void
err_quit(const char *fmt, ...)
{
	va_list		ap;

	va_start(ap, fmt);
	err_doit(0, 0, fmt, ap);
	va_end(ap);
	exit(1);
}

/*
 * Print a message and return to caller.
 * Caller specifies "errnoflag".
 */
static void
err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
	char	buf[MAXLINE];

	vsnprintf(buf, MAXLINE-1, fmt, ap);
	if (errnoflag)
		snprintf(buf+strlen(buf), MAXLINE-strlen(buf)-1, ": %s",
		  strerror(error));
	strcat(buf, "\n");
	fflush(stdout);		/* in case stdout and stderr are the same */
	fputs(buf, stderr);
	fflush(NULL);		/* flushes all stdio output streams */
}
void pr_mask(const char *str)
{
    sigset_t sigset;
    int errno_save;
    errno_save=errno;
    if(sigprocmask(0,NULL,&sigset))
    {
        err_ret("sigprocmask error");
    }
    else
    {
        printf("%s",str);
        if(sigismember(&sigset,SIGINT))
            printf("SIGINT");
        if(sigismember(&sigset,SIGQUIT))
            printf("QUIT");
        if(sigismember(&sigset,SIGUSR1))
            printf("USR1");
        if(sigismember(&sigset,SIGUSR2))
            printf("USR2");
        printf("\n");
    }
    errno =errno_save;
}
//主要讲8.9节中说到的五个例程的实现
//分别是TELLWAIT、TELL_PARENT、TELL_CHILD、WAIT_PARENT、WAIT_CHILD


static volatile sig_atomic_t sigflag;
static sigset_t newmask,oldmask,zeromask;

static void sig_usr(int signo)
{
    sigflag=1;
}
void TELL_WAIT(void)
{
    if(signal(SIGUSR1,sig_usr)==SIG_ERR)
        err_sys("signal sigusr1 error");
    if(signal(SIGUSR2,sig_usr)==SIG_ERR)
        err_sys("signal sigusr2 error");
    sigemptyset(&newmask);
    sigemptyset(&zeromask);
    sigaddset(&newmask,SIGUSR1);
    sigaddset(&newmask,SIGUSR2);

    if(sigprocmask(SIG_BLOCK,&newmask,&oldmask)<0)
        err_sys("SIG_BLOCK error");
}

void TELL_PARENT(pid_t pid)
{
    kill(pid,SIGUSR2);
}
void WAIT_PARENT(void)
{
    while(sigflag==0)
        sigsuspend(&zeromask);
    sigflag=0;
    if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0)
        err_sys("SIG_SETMASK error");
}
void TELL_CHILD(pid_t pid)
{
    kill(pid,SIGUSR1);
}
void WAIT_CHILD(void)
{

    while(sigflag==0)
        sigsuspend(&zeromask);
    sigflag=0;
    if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0)
        err_sys("SIG_SETMASK error");
}
void clr_fl(int fd,int flags)
{
    int val;
    if((val=fcntl(fd,F_GETFL,0))<0)
        err_sys("fcntl F_GETFL error");
    val = ~flags;
    if(fcntl(fd,F_SETFL,val)<0)
        err_sys("fcntl F_SETFL error");
}
void set_fl(int fd,int flags)
{
    int val;
    if((val=fcntl(fd,F_GETFL,0))<0)
        err_sys("fcntl F_GETFL error");
    val |=flags;
    if(fcntl(fd,F_SETFL,val)<0)
        err_sys("fcntl F_SETFL error");
}
int lock_reg(int fd,int cmd,int type,off_t offset ,int whence , off_t len)
{
    struct flock lock;


    lock.l_type=type;
    lock.l_start=offset;
    lock.l_whence=whence;
    lock.l_len=len;

    //为避免每次分配flock结构的繁琐，将其动作封装成函数，一起运用，提高效率
    //，在以后会用到
    return fcntl(fd,cmd,&lock);
}

pid_t lock_test(int fd,int type,off_t offset ,int whence ,off_t len)
{   
    //函数的作用是测试一把锁，如果存在一把锁，它阻塞由参数指定的锁请求，则此函数返回持有
    //这把锁的进程的进程ID，否则此函数返回0。
    struct flock lock;
    lock.l_type=type;
    lock.l_start=offset;
    lock.l_whence=whence;
    lock.l_len=len;
    if(fcntl(fd,F_GETLK,&lock)<0)//利用fcntl添加记录锁，并且记录状态
        err_sys("fcntl error");
    if(lock.l_type==F_UNLCK)
        return 0;
    return (lock.l_pid);
}
ssize_t readn(int fd,void *ptr ,size_t n)
{
    size_t      nleft;
    ssize_t     nread;


    nleft=n;
    while(nleft>0)
    {
        if(nread=read(fd,ptr,nleft)<0)
        {
            if(nleft==0)
                return -1;
            else 
                break;
        }
        else if(nread==0)
        {
            break;//EOF error
        }
        nleft-=nread;
        ptr+=nread;
    }
    return (n=nleft);//return >=0
}

//改进版的read 和write函数。以为传进数据，读写指定的 数据
ssize_t writen(int fd, const void * ptr ,size_t n)
{
    size_t      nleft;
    ssize_t     nwriten;

    nleft=n;

    while(nleft>0)
    {
        if(nwriten=write(fd,ptr,nleft)<0)
        {
            if(nleft==n)
                return -1;
            else
                break;
        }
        else if(nwriten==0)
            break;
        nleft-=nwriten;
        ptr+=nwriten;
    }
    return (n-nleft);
}
