#include"apue.h"
int globval =6;

int main(int argc,char ** argv)
{
	int val;
	pid_t pid ;
	val=88;
	printf("before vfork\n");
	if((pid=vfork())<0)
	{
		err_sys("vfork error");
	}
	else if(pid==0)
	{
		globval++;
		val++;
		_exit(0);
	}
	printf("pid=%ld,glob=%d,val=%d\n",(long)getpid(),globval,val);
	exit(0);




}
