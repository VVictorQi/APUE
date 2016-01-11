#include"apue.h"
//通过调用fork()函数两次来解决僵尸进程的出现
int main(void)
{
	pid_t pid;
	if((pid=fork())<0)
		err_sys("fork error");
	else if(pid==0)
	{	
		if((pid=fork())<0)
			err_sys("fork error");
		else if(pid>0)
			exit(0);
		sleep(2);
		printf("second child ,parent pid =%ld\n, self pid=%ld\n",(long )getppid(),(long)getpid());
		exit(0);
	}
	if(waitpid(pid ,NULL,0)!=pid)	
		err_sys("waitpid error");
//we're the parent  we continue executing ,knowing that we are not the parent of the second child



	exit(0);
}
