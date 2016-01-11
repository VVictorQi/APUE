#include"apue.h"
#include<fcntl.h>
int  main(int argc ,char **argv)
{
	if(open("tempfile",O_RDWR)<0)
		err_sys("open tempfile file faied");
	if(unlink("tempfile")<0)
		err_sys("unlink error");
	printf("file unlinked \n");
	sleep(10);
	printf("done\n");
	exit(0);







}

