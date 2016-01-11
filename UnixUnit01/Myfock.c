#include "apue.h"
int main(int argc,char ** argv)
{	
	printf("hello world from process Id %ld\n",(long)getpid());
	exit(0);

}
