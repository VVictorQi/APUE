//进程会计 统计进程运行时间
/*
   获取会计记录，又离开那个特例，对于init这样的永远不停止的进程不进行会计记录统计
   在会计文件中记录的顺序对应着应用程序终止的顺序
   会计记录对应于进程而不是程序
 */


#include"apue.h"
int main(void)
{
	pid_t pid;
	if(pid=fork()<0)
	{
		err_sys("fork error");
	}
	else if(pid!=0)
	{
		sleep(2);
		exit(2);
	}
	if(pid=fork()<0)
	{
		err_sys("fork error");
	}
	else if(pid!=0)
	{
		sleep(4);
		abort();
	}
	if(pid=fork()<0)
	{
		err_sys("fork error");
	}
	else if(pid!=0)
	{
		execl("/bin/dd","dd","if=/etc/passwd","of=/dev/null",NULL);//对于dd of 而言是对一个文件块进行扩张，将if对应的文件扩大of对应的大小
		exit(7);//should get here
	}

	if(pid=fork()<0)
	{
		err_sys("fork error");
	}
	else if(pid!=0)
	{
		sleep(8);
		exit(0);
	}

	sleep(6);
	kill(getpid(),SIGKILL);
	exit(6);
}
