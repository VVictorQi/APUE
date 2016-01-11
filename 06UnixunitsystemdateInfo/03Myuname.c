#include"apue.h"
#include<sys/utsname.h>
#include<unistd.h>
int main(void)
{
	int i;
	struct utsname myuname;
	if((i=uname(&myuname))<0)
		err_sys("uname error");
	else
		printf("%s\t%s\t%s\t%s\t%s",myuname.sysname,myuname.nodename,myuname.version,myuname.release,myuname.machine);



//	char buf[64];
//	if((gethostname(&buf,strlen(buf))<0)
//		err_sys("gethostname error");










	
	exit(0);
}
