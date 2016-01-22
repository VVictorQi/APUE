/*************************************************************************
	> File Name: 15-10_31MySignalContral.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Fri 22 Jan 2016 06:49:42 PM CST
 ************************************************************************/

#include "apue.h" 
#define BUFFSIZE 1024                          
static void sig_tstp(int signo)             
{                                                
     sigset_t mask;                               
     sigemptyset(&mask);                          
     sigaddset(&mask,SIGTSTP);                            
     sigprocmask(SIG_UNBLOCK,&mask,NULL);         
     signal(SIGTSTP,SIG_DFL);                     
     kill(getpid(),SIGTSTP);                      
                                                             
                                                                
    signal(SIGTSTP,sig_tstp);                    
}                                                                       
int main(void)                          
{                                                
    int n;                                       
    char line[BUFFSIZE];                         
    if(signal(SIGTSTP,sig_tstp)==SIG_DFL)        
        signal(SIGTSTP,sig_tstp);                
    while((n=read(STDIN_FILENO,line,BUFFSIZE))>0)
        if(write(STDOUT_FILENO,line,n)!=n)
            err_sys("write error");
    if(n<0)
        err_sys("read error");
    exit(0);
}
