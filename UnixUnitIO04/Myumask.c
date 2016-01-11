#include"apue.h"
#include<fcntl.h>
#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)
int main(int argc , char ** argv)
{
	umask(0);
	if(creat("foo",RWRWRW)<0)
		err_sys("creat error for foo");
	umask(S_IRUSR|S_IWUSR|S_IXUSR|S_IROTH|S_IWOTH);
	if(creat("bar",RWRWRW)<0)
		err_sys("create error for bar");
	exit(0);









}
