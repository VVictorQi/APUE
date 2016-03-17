#include"./epollHeader.h"
int main(int argc, char *argv[])
{
    //用户连接的服务器 IP + port
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 创建socket
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock < 0) { perror("sock error"); exit(-1); }
    // 连接服务端
    if(connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("connect error");
        exit(-1);
    }

    // 创建管道，其中fd[0]用于父进程读，fd[1]用于子进程写
    int pipe_fd[2];
    if(pipe(pipe_fd) < 0) { perror("pipe error"); exit(-1); }

    // 创建epoll
    int epfd = epoll_create(EPOLL_SIZE);
    if(epfd < 0) { perror("epfd error"); exit(-1); }
    static struct epoll_event events[2]; 
    //将sock和管道读端描述符都添加到内核事件表中
    addfd(epfd, sock, true);
    addfd(epfd, pipe_fd[0], true);
    // 表示客户端是否正常工作
    bool isClientwork = true;

    // 聊天信息缓冲区
    char message[BUF_SIZE];

    // Fork
    int pid = fork();
    if(pid < 0) { perror("fork error"); exit(-1); }
    else if(pid == 0)      // 子进程
    {
        //子进程负责写入管道，因此先关闭读端
        close(pipe_fd[0]); 
        printf("Please input 'exit' to exit the chat room\n");

        while(isClientwork){
            bzero(&message, BUF_SIZE);
            fgets(message, BUF_SIZE, stdin);

            // 客户输出exit,退出
            if(strncasecmp(message, EXIT, strlen(EXIT)) == 0){
                isClientwork = 0;
            }
            // 子进程将信息写入管道
            else {
                if( write(pipe_fd[1], message, strlen(message) - 1 ) < 0 )
                 { perror("fork error"); exit(-1); }
            }
        }
    }
    else  //pid > 0 父进程
    {
        //父进程负责读管道数据，因此先关闭写端
        close(pipe_fd[1]); 

        // 主循环(epoll_wait)
        while(isClientwork) {
            int epoll_events_count = epoll_wait( epfd, events, 2, -1 );
            //处理就绪事件
            for(int i = 0; i < epoll_events_count ; ++i)
            {
                bzero(&message, BUF_SIZE);

                //服务端发来消息
                if(events[i].data.fd == sock)
                {
                    //接受服务端消息
                    int ret = recv(sock, message, BUF_SIZE, 0);

                    // ret= 0 服务端关闭
                    if(ret == 0) {
                        printf("Server closed connection: %d\n", sock);
                        close(sock);
                        isClientwork = 0;
                    }
                    else printf("%s\n", message);

                }
                //子进程写入事件发生，父进程处理并发送服务端
                else { 
                    //父进程从管道中读取数据
                    int ret = read(events[i].data.fd, message, BUF_SIZE);

                    // ret = 0
                    if(ret == 0) isClientwork = 0;
                    else{   // 将信息发送给服务端
                      send(sock, message, BUF_SIZE, 0);
                    }
                }
            }//for
        }//while
    }

    if(pid){
       //关闭父进程和sock
        close(pipe_fd[0]);
        close(sock);
    }else{
        //关闭子进程
        close(pipe_fd[1]);
    }
    return 0;
}