#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/stat.h>

#define BUFSIZE 10  //流量速率
#define MAXTOKEN 1024 //令牌上线
static volatile int token=0;//积攒的令牌数量

static  void alarm_handler(int s)
{
	alarm(1);
	if(token<MAXTOKEN)
	{
		token++;
	}
}

int main(int  argc ,char ** argv)
{
	int fd=-1;
	char buf[BUFSIZE] ="";
	ssize_t readsize= -1;
	ssize_t writesize =-1;
	size_t off =0;

	if(argc<2)
	{
		fprintf(stderr,"usage %s<filepath>\n",argv[0]);
		return 1;
	}
	do 
	{
		fd=open(argv[1],O_RDONLY);
		if(fd<0)
		{
			if(EINTR!=errno)
			{
				perror("open()");
				goto e_open;
			}
		}
	}while(fd<0);
	signal(SIGALRM,alarm_handler);
	alarm(1);
	while(1)
	{
		while(token<=0)
		{	pause();}
		token--;
		while((readsize=read(fd,buf,BUFSIZE))<0)
		{
			if(readsize<0)
			{
				if(EINTR==errno)
					continue;
				perror("read()");
				goto e_read;
			}
		}
		if(!readsize)
			break;
	}
	
	off=0;
	do
	{
		writesize=write(1,buf+off,readsize);
		off+=writesize;
		readsize-=writesize;
	}while(readsize>0);

	close(fd);
	return 0;

e_read:
	close(fd);
e_open:
	return 1;


	


}


























