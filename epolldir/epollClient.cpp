#include"./epollHeader.h"
int main(int argc ,char ** argv)
{
	struct 	 sockaddr_in  serverAddr;
	serverAddr.sin_family=PF_INET;//采用IPV4协议
	serverAddr.sin_port=htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr=inet_addr(SERVER_IP);


	//创建socket
	int sock=socket(PF_INET,SOCK_STREAM,0);

	if(sock<0)
	{
		perror("create socket error");
		exit(-1);
	}
	if(connect(sock, (struct sockaddr*)&serverAddr,sizeof(serverAddr))<0)
	{
		perror("connect error");
		exit(-1);//注意在这里可以采用超时重连技术、这里我们下次再说
	}
	//创建管道
	int pipe_fd[2];//分别代表读写控制
	if(pipe(pipe_fd)<0)
	{
		perror("pipe error");
		exit(-1);
	}

	//在这里开始创建epoll

	int epfd=epoll_create(EPOLL_SIZE);
	if(epfd<0)
	{
		perror("create epfd error");
		exit(-1);
	}
	static struct epoll_event  events[2];
 	//将sock和管道描述符添加到内核事件表中
 	addfd(epfd,sock,true);
 	addfd(epfd,pipe_fd[0],true);
 	bool isClientwork= true;



 	char message[BUF_SIZE];

 	//开始fork形成多进程环境

 	pid_t pid;
 	if((pid=fork())<0)
 	{
 		perror("fork() error");
 		exit(-1);
 	}
 	else if(pid==0)
 	{
 		close(pipe_fd[0]);//关闭读端口，开始写数据。
 		printf("Please input 'exit 'to exit the char room");

 		while(isClientwork)
 		{
 			bzero(&message,BUF_SIZE);
 			fgets(message,BUF_SIZE,stdin);
 			//当客户端输入exit 退出
 			if(strncasecmp(message,EXIT,strlen(EXIT))==0)
 				isClientwork=false;
 			else
 			{
 				if(write(pipe_fd[1],message,strlen(message)-1)<0)
 				{
 					perror("数据读取错误");
 				}
 			}
 		}
 	}

 	else
 	{
 		close(pipe_fd[1]);

 		while(isClientwork)
 		{
 			int epoll_events_count=epoll_wait(epfd,events,2,-1);
 			//开始处理就绪事件
 			for(int i=0;i<epoll_events_count;++i)
 			{
 				bzero(&message,BUF_SIZE);
 				if(events[i].data.fd==sock)
 				{
 					int ret =recv(sock,message,BUF_SIZE,0);
 					if(ret==0)
 					{
 						printf("Servet closed connection :%d\n",sock );
 						close(sock);
 						isClientwork=false;
 					}

 				}
 				else
 				{
 							{
 					int ret =read(events[i].data.fd,message,BUF_SIZE);
 					if(ret==0)
 						isClientwork=false;
 					else//向服务端发送消息
 						send(sock,message,BUF_SIZE,0);
 				}
 				}
 			}
 		}
 	}
 	if(pid)
 	{
 		close(pipe_fd[0]);
 		close(epfd);
 	}
 	else
 		close(pipe_fd[1]);
 	return 0;
}
