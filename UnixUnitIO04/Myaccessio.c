#include"apue.h"
#include<fcntl.h>
int main(int argc, char ** argv)
{
	if(argc!=2)
		err_quit("\tusage:%s <pathname>",argv[0]);
	if(access(argv[1],R_OK)<0)
		err_ret("\taccess error for %s",argv[1]);
	else
		printf(" \tread access OK\n");
	if(open(argv[1],O_RDONLY)<0)
		err_ret("\topen error for %s",argv[1]);
	else
		printf("\topen for reading OK \n");
	exit(0);
}
