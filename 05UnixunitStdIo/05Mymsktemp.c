#include"apue.h"
#include<errno.h>
void make_temp(char *);
int main(void)
{
	char good_template[]="/tmp/dirXXXXXX";
	char *bad_template="/tmp/dirXXXXXX";
	printf("trying to create first temp file...\n");
	make_temp(good_template);
	printf("trying to create second temp file...\n");
	make_temp(bad_template);
	exit(0);
}

void make_temp(char * template)
{
	int fd;
	struct stat sbuf;
	if((fd=mkstemp(template))<0)
		err_sys("can't create tempfile");
	printf("temp name =%s\n",template);
	close(fd);
	if((stat(template,&sbuf))<0)
	{	
		if(errno==ENOENT)
			printf("file doesn't exist !\n");
		else
			err_sys("stat failed");

	}
	else
	{
		printf("file exist \n");
		unlink(template);
	}
}
