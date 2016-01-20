#include<signal.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<assert.h>
static void sigusr(int signo);//信号处理函数
int main(void)
{
	printf("nihaoshije");
	if(signal(SIGINT,sigusr)==SIG_ERR)
		printf("can't catch SIGUSR1\n");
	if(signal(SIGUSR2,sigusr)==SIG_ERR)
		printf("can't catch SIGUSR2\n");	
//	assert(signal(SIGINT,sigusr)==sigusr);
////	if((signal(SIGINT,SIG_IGN))!=SIG_IGN)
//		signal(SIGINT,sigusr);
//	if((signal(SIGUSR2,SIG_IGN))!=SIG_IGN)
//		signal(SIGUSR2,sigusr);

	for( ; ; )
		pause();
	//无限循环语句，在早期程序员年代流传着这样比while(1)循环更有效率的说法，事实上也正是如此，但是在现在优化的结果两者是相同
		

}

static void  sigusr(int s)
{
	printf("i come back");
	if(s == SIGINT)
		printf("SIGINT\n");
	else if(s == SIGUSR2)
		printf("SIGUSR2\n");
	else 
		printf("recevied  %d \n",s);
}

