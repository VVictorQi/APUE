#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

void handler(int num)
{
	printf("recv num:%d \n",num);
	if(num==SIGQUIT)
	{
		exit(0);
	}
}
int main(void)
{
	signal(SIGINT,handler);
	signal(SIGQUIT,handler);
	while(1)
	{
		pause();										
	}
	printf("main   ...begin\n");
	exit(0);
}
