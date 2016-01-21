/*************************************************************************
	> File Name: 3-10_6MySIGCLD.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Thu 21 Jan 2016 01:40:29 PM CST
 ************************************************************************/

#include<stdio.h>
#include<apue.h>                        
#include<sys/wait.h>                    
static void  sig_cld(int);              

int main()                              
{                                       
   
    pid_t pid;                           
    if(signal(SIGCLD,sig_cld)==SIG_ERR) 
        perror("signal errror");       
    if((pid==fork())<0)                     
        perror("fork error" );
    else if(pid==0)
    {
        sleep(2);
        _exit(0);
    }
    pause();
    exit(0);
}
static void sig_cld(int signo)          
{
    pid_t pid;                          
    int status;                         
    printf("SIGCLD recevied \n");       
    if(signal(SIGCLD,sig_cld)==SIG_ERR)
        perror("signal error \n");
    if((pid==wait(&status))<0)
        perror("wait error\n");
    printf("pid=%d\n",pid);
 }

