/*************************************************************************
	> File Name: 06-15_14MySimpleCp.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Thu 18 Feb 2016 11:48:30 PM CST
 ************************************************************************/

#include "apue.h" 
#include<ctype.h>
int main(void)
{
    int         c;
    while((c=getchar())!=EOF)
    {
        if(isupper(c))
            c=tolower(c);
        if(putchar(c)==EOF)
            err_sys("output error");
        if(c=='\n')
            fflush(stdout);//对于标准输出来讲通常是行缓冲的，而提示并不包含换行符，需要调用fflush
    }
    exit(0);
}
