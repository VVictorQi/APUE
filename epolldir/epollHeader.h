/*************************************************************************
	> File Name: epollHeader.h
	> Author:Victor Qi 
	> Mail:Victor_QiGetFuture@163.com 
	> Created Time: Mon 07 Mar 2016 09:46:15 PM CST
 ************************************************************************/

#ifndef _EPOLLHEADER_H
#define _EPOLLHEADER_H
#include<iostream>
#include<list>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
using namespace std;
list<int> client_list;
#define SERVER_IP    "127.0.0.1"
#define  SERVER_PORT   10086
#define  EPOLL_SIZE   5000
#define  BUF_SIZE     0xFFFF
#define  SERVER_WELCOME  "Weclome  you join  to  the chat room ! your ID is :Client  #%d"
#define  SERVER_MESSAGE  "Client ID %d say>> %s"
#define  EXIT         "EXIT"
#define  CAUTION      "There is only one int the char room !"

//对此设置阻塞模式为非阻塞模式
int setnoblocking(int sockfd)
{
    fcntl(sockfd,F_SETFL,fcntl(sockfd,F_GETFD,0)|O_NONBLOCK);
    return 0;
}
//
//
void addfd(int epollfd,int fd , bool enable_et)
{
    struct epoll_event ev;
    ev.data.fd=fd;
    ev.events=EPOLLIN;
    if(enable_et)
        ev.events=EPOLLIN|EPOLLET;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
    printf("fd added to epoll!\n\n");
}
int sendBroadcastmessage(int clientfd)
{
    char buf[BUF_SIZE],message[BUF_SIZE];
    bzero(buf,BUF_SIZE);//将缓冲区清零
    bzero(message,BUF_SIZE);


    //开始接收信息
    //
    printf("read form client (clientID=%d)\n",clientfd);
    int len=recv(clientfd,buf,BUF_SIZE,0);

    if(len==0)
    {
        close(clientfd);
        client_list.remove(clientfd);//移除客户端
        printf("ClientID=%d closed .\n now there are %d client  int char room\n",clientfd,(int)client_list.size());
    }
    else
    {
        if(client_list.size()==1)
        {
            send(clientfd,CAUTION,strlen(CAUTION),0);
            return len;
        }
        sprintf(message,SERVER_MESSAGE,clientfd,buf);

        list<int>::iterator  it;
        for(it=client_list.begin();it!=client_list.end();++it)
        {
            if(*it!=clientfd)
            {
                if(send(*it,message,BUF_SIZE,0)<0)
                {
                    perror("error");
                    exit(-1);
                }
            }
        }
    }
    return len;
}
#endif
