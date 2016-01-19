#include"apue.h"
#include<unistd.h>
#include<stdlib.h>
static void sigusr(int signo);//信号处理函数
int main(void)
{
	if(signal(SIGINT,sigusr)==SIG_ERR)
		err_sys("can't catch SIGUSR1\n");
	if(signal(SIGUSR2,sigusr)==SIG_ERR)
		err_sys("can't catch SIGUSR2\n");
	
	/*if((signal(SIGINT,SIG_IGN))!=SIG_IGN)
		signal(SIGINT,sigusr);
	if((signal(SIGUSR2,SIG_IGN))!=SIG_IGN)
		signal(SIGUSR2,sigusr);
*/
//	for( ; ; )//无限循环语句，在早期程序员年代流传着这样比while(1)循环更有效率的说法，事实上也正是如此，但是在现在优化的结果两者是相同的
	while(1)
	{sleep(100);}
	exit(0);
}

static void  sigusr(int signo)
{
	printf("right finished");
	if(signo == SIGINT)
		printf("SIGINT\n");
	else if(signo == SIGUSR2)
		printf("SIGUSR2\n");
	else 
		err_dump("recevied  %d \n",signo);
}

