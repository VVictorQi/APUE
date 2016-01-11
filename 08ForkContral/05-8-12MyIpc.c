//To solve the race condition 解决竞争关系和轮询的情况
//在unix中有专门的IPC通信或者管道符操作。
#include"apue.h"
static void charactatime(char *);
int main(void)
{
	pid_t pid ;
	if((pid= fork())<0)
		err_sys("fork error");
	else if(pid==0)
		charactatime("output from child\n");
	else 
		charactatime("output from parent\n");
	

	exit(0);
}
static void charactatime(char*str)
{
	char *ptr;
	int c;
	setbuf(stdout,NULL);
	for(ptr=str;(c=*ptr++)!=0;)
		putc(c,stdout);
}
//竞争策略对于完善的linux系统来说已经完善的很好了，对于一些宏，或者函数也不再需要
