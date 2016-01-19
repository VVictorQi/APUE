#include"apue.h"
#include<pwd.h>

static void my_alarm(int signo)
{
	struct passwd * rootptr;
	printf("int the signal function \n");
	if((rootptr=getpwnam("root"))==NULL)
		err_sys("getpwnam(root) error\n");
	alarm(1);
}


int main(void)
{
	struct passwd *ptr;
	signal(SIGALRM,my_alarm);
	alarm(1);
	for(;;)
	{
		if((ptr=getpwnam("victor"))==NULL)
			err_sys("getpwnam(victor) error\n");
		if(strcmp(ptr->pw_name,"victor")!=0)
			printf("return value corrupted!,pw_name=%s\n",ptr->pw_name);
	}
}
