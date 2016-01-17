#include <signal.h>
#include"apue.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#define ERR_EXIT(m) \
	do\
	{ \
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)

void handler(int num)
{
	if(num==SIGUSR1)
	{
		printf("解除阻塞成功recv num:%d \n",num);
		sigset_t bset;
		sigemptyset(&bset);
		sigaddset(&bset,SIGINT);
		sigaddset(&bset,SIGRTMIN);
		sigprocmask(SIG_UNBLOCK,&bset,NULL);
	}
	else if(num==SIGINT||num==SIGRTMIN)
	{
		printf("recv num:%d \n",num);
	}
	else
	{
		exit(0);
	}
}
int main(void)
{
	pid_t pid ;
	struct sigaction act;
	act.sa_handler =handler;
	act.sa_flags =SA_SIGINFO;
	//注册非实时信号,处理函数
	if(sigaction(SIGINT,&act,NULL)<0)
	{
		ERR_EXIT("sigaction SIGINT");

	}
	//注册实时信号的处理函数
	if(sigaction(SIGRTMIN,&act,NULL)<0)
	{
		ERR_EXIT("sigaction SIGRTMIN");

	}
	//注册用户自定义的自定义信号SIGUSR1函数
	if(sigaction(SIGUSR1,&act,NULL)<0)
	{
		ERR_EXIT("sigaction SIGUSR1");
	}
	//把SIGINT和SIGRTMIN均添加到本进程的阻塞状态中
	sigset_t bset;
	sigemptyset(&bset);
	sigaddset(&bset,SIGINT);
	sigaddset(&bset,SIGRTMIN);
	sigprocmask(SIG_BLOCK,&bset,NULL);
	if((pid=fork())<0)
	{
		err_sys("fork error");
	}
	else if(pid==0)
	{	
		int ret=0;
		int i=0;
		union sigval value;
		value.sival_int=201;
		//发三次不稳定信号
		for(i=0;i<3;i++)
		{
			ret=sigqueue(getppid(),SIGINT,value);
			if(ret!=0)
			{
				printf("发送不可告信号失败ret:%d,errno :%d \n",ret,errno);
				exit(0);
			}
			else
			{
				printf("发送不可靠信号ok");
			}

		}
		value.sival_int=301;
		for(i=0;i<3;i++)
		{
			ret=sigqueue(getppid(),SIGRTMIN,value);
			if(ret!=0)
			{
				printf("发送可告信号失败ret:%d,errno :%d \n",ret,errno);
				exit(0);
			}
			else
			{
				printf("发送可靠信号ok");
			}
		}
		kill(getppid(),SIGUSR1);
	}
	for(;;)
	{
		sleep(1);
	}
	printf("main.......\n");
}















