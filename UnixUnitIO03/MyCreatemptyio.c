#include"apue.h"
#include<fcntl.h>
char buf1[]="qwertyuiop";
char buf2[]="QWERTYUIOP";
int main(void)
{
	int fd;
	if((fd=creat("myfile.txt",FILE_MODE))<0)
		err_sys("creat error");
	if(write(fd,buf1,10)!=10)
		err_sys("write error");
	if(lseek(fd,16384,SEEK_SET)==-1)
		err_sys("lseek error");
	if(write (fd ,buf2,10)!=10)
		err_sys("write error");



	
	//创建一个空洞文件，空洞文件是多线程的支持点之一，因为空洞文件的存在才能支持多线程断点续传


	exit(0);






}
