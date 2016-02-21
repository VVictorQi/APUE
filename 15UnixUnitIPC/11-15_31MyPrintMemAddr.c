/*************************************************************************
	> File Name: 11-15_31MyPrintMemAddr.c
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Sun 21 Feb 2016 02:09:38 PM CST
 ************************************************************************/

#include "apue.h" 
#include<sys/shm.h>
#define  ARRAY_SIZE     40000
#define  MALLOC_SIZE    100000
#define  SHM_SIZE       100000
#define  SHM_MODE       0600   //rw

char   array[ARRAY_SIZE];



int main(void)
{
    int         shmid;
    char        *ptr,*shmptr;


    printf("array[] from %p to %p \n",(void*)&array[0],(void*)&array[ARRAY_SIZE]);
    printf("stack around %p\n",(void*)&shmid);


    if((ptr=malloc(MALLOC_SIZE))==NULL)
        err_sys("malloc error");
    printf("malloced   from %p to %p \n",(void*)&ptr,(void*)ptr+MALLOC_SIZE);


    if((shmid=shmget(IPC_PRIVATE,SHM_SIZE,SHM_MODE))<0)
        err_sys("shmget  error");
    //利用shmget获得一个共享内存标识符，现在shmid等同于共享内存标志。
    if((shmptr=shmat(shmid,0,0))==(void*)-1)
        err_sys("shmat error");
    //  利用shmat连接到他的地址空间中
    printf("shared memory  attached from %p  to %p \n",(void*)shmptr,(void*)shmptr+SHM_SIZE);

    if(shmctl(shmid,IPC_RMID,0)<0)//利用shmctl对共享内存进行操作。其中IPC_RMID是删除标志
        err_sys("shmctl error");
}
