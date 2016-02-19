/*************************************************************************
	> File Name: 10-15_19MyStdIoCoprocessadd.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Fri 19 Feb 2016 11:38:53 PM CST
 ************************************************************************/

#include "apue.h" 

int main(void)
{
    int n, numone,numtwo;
    char line[MAXLINE];


    while(fgets(line,MAXLINE,stdin)!=NULL)
    {
        if(sscanf(line,"%d%d",&numone,&numtwo)==2)
        {
            if(printf("%d\n",numone+numtwo)==EOF)
                err_sys("printf error");
        }
        else
        {
           if( printf("invalid args ")==EOF)
                err_sys("printf error");
        }
    }
    exit(0);
}
//在这里对于此程序不做编译，因为此程序是对于第八个程序的 标准库改写系统调用。
//假如这样的程序被第九个实例调用，那么程序将不会被执行,因为标准IO库具有缓冲区，
//而标准输入是一个管道，所以标准IO库默认是全缓冲的。标准输出也是如此。当本实例从其
//标准输入读取而发生阻塞时，09实例也发生阻塞，这样就产生了死锁现象.但是这样可以修改
//此程序来解决，通过使用setvbuf来接觉此问题，显式调用setbuf可以在程序键入回车是，立即刷新缓冲区
//但是倘若调用的是awk呢，我们没有awk源码，怎么解决，这个在19章提供了一个解决方案终端
