#include"apue.h"
#define PSCMD  "ps -o pid,ppid,state,tty,command"
int main(void)
{
	pid_t pid;
	if((pid=fork())<0)
		err_sys("fork error");
	else if(pid==0)
		exit(0);
	//父进程
	sleep(4);
	system(PSCMD);
	exit(0);
}
