#include"apue.h"
#include<sys/wait.h>
static void sig_int(int signo);
int main(int argc,char **argv)
{
	char buf[MAXLINE];// from apue.h referrnce
	pid_t pid;
	int status;
	if(signal(SIGINT,sig_int)==SIG_ERR)
		err_sys("singal error");
	printf("%%");
	while(fgets(buf,MAXLINE,stdin)!=NULL)
	{
		if((buf[strlen(buf)-1])=='\n')
			buf[strlen(buf)-1]=0;//relplace newline with null;
		if ( (pid = fork() ) < 0)
		{
			err_sys("fork error");
		}
		else if(pid==0)
		{
			execlp(buf,buf, (char*)0);
			err_ret("couldn't execute :%s ",buf);
			exit(127);
		}


		if((pid=waitpid(pid,&status,0))<0)
			err_sys("waitpaid error");
		printf("%%");
	}
	exit(0);
}


void sig_int(int signo)
{
	printf("interrupt\n%%");
}
