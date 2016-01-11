//#include<stdio.h>
#include"apue.h"
//#include<unistd.h>
//#include<string.h>
//#include<stderr.h>
#include<dirent.h>
//#include<myerror.h>//这个文件不可用
int main(int argc,char **argv)
{
	DIR 	       *dp;
	struct dirent *dirp;
	if(argc!=2)
		err_quit("usage:ls directory_name");			
		//	printf("error");
	if((dp=opendir(argv[1]))==NULL)
		err_sys("can't open %s",argv[1]);
		//	printf("can't open %s",argv[1]);
	while((dirp=readdir(dp))!=NULL)
		printf("%s\n",dirp->d_name);
	closedir(dp);
	exit(0);
		

}
