#include"apue.h"
int main(int argc,char **argv,char ** env)
{
	int i;
	char **ptr;
	for(i=0;i<argc;i++)
		printf("argv[%d]:%s\n",argv[i]);
	for(;*env++!=0;)
		printf("%s\n",*env);
	exit(0);
}
