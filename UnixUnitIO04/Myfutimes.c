#include"apue.h"
#include<fcntl.h>
int main(int argc,char **argv)
{
	int		i,fd;
	struct stat buf;
	struct timespec times[2];
	for(i=1;i<argc;i++)
	{
		if(stat(argv[i],&buf)<0)//fetch the current time
		{
			err_ret("%s:stat error",argv[i]);
			continue;
		}
		if((fd=open(argv[i],O_RDWR|O_TRUNC))<0)
		{
			err_ret("%s:open error ",argv[i]);
			continue;
		}	
		times[0]=buf.st_atim;
		times[1]=buf.st_mtim;
		if(futimens(fd,times)<0)//reset times
			err_ret("%s:futimes error",argv[i]);
		close(fd);	
	}
	exit(0);
}
