#include"apue.h"
int main(void)
{
	char*ptr;
	size_t size;
	if(chdir("/usr/include")<0)
		err_sys("chdir error");
	ptr = path_alloc(&size);
	if(getcwd(ptr,size)==NULL)
		err_sys("getcwd error");
	printf("cwd=%s\n",ptr);
	exit(0);


}
