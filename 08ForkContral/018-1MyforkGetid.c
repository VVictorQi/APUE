#include"apue.h"
int globval=6;
char buf[]="a write t ostdout\n";
int main(int argc,char **argv)
{
	int var;
	pid_t pid;
	var =88;
	if(write(STDOUT_FILENO,buf,sizeof(buf)-1)!=sizeof(buf)-1)
		err_sys("write error");
	printf("before fork\n");
	if(pid=fork()<0)
	{
		err_sys("fork error");
	}
	else if(pid==0)
	{
		globval++;
		var++;
	}
	else 
	{
		sleep(2);
	}
	
	printf("pid=%ld \t glob=%d \t val =%d\n ",(long)getpid(),globval,var);


	exit(0);
}

