#include"apue.h"
#include<fcntl.h>
int main(void)
{
	if(chdir("/tmp")<0)
		err_sys("chdir error");
	printf("chdir to /tmp successed\n");
	exit(0);





}
