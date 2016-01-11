#include"apue.h"


int main(int argc , char** argv)
{
	struct stat buf;
	if(stat("foo",&buf)<0)
		err_sys("stat error for foo");
	if(chmod("foo",(buf.st_mode & ~S_IXGRP)|S_ISGID)<0)
		err_sys("chmod error for foo");


	if(chmod("bar",S_IRUSR|S_IWUSR|S_IRWXG)<0)
		err_sys("chmod error for bar");

	exit(0);

}
