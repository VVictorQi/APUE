//To solve the race condition 解决竞争关系和轮询的情况
//在unix中有专门的IPC通信或者管道符操作。
#include"apue.h"
static void charactatime(char *);
int main(void)
{
	pid_t pid ;
	TELL_WAIT();
	if((pid= fork())<0)
		err_sys("fork error");
	else if(pid==0)
	{
		charactatime("output from child\n");
		WAIT_PARENT();
	}
	else 
	{
		charactatime("output from parent\n");
		TELL_CHILD(pid);
	}

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
