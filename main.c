#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <string.h>

#include "socket.h"
#include "request.h"
#include "epoll.h"

//调试模式开关 0=关 1=开
#define	_DEBUG	1
//以下三个码分别代表：成功、正常结束、失败
#define	_SUCCODE 0
#define	_ENDCODE 1
#define _ERRCODE (-1)

#define EP_CREATE_SIZE 2
#define MAX_EVENT_NUMBER 2
//超时时间10秒
#define TIMEOUT (100 * 1000)
//是否开启ET模式
#define ENABLE_ET 0


int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        fprintf(stderr, "paras error, need listened port \n");
        return _ERRCODE;
    }
    else if (_DEBUG)
    {   
        printf("Server is starting... \n");
        printf("Listened port is %s \n", argv[1]);
    }
    
    int listenfd, connfd;

    //创建 socket，返回监听描述符
    listenfd = Open_listenfd(argv[1]);
    if (listenfd < 0)
    {
        printf("Open listenfd error \n");
        return _ERRCODE;
    }

    /**
     * 1.accept read write 改为非阻塞
     * 2.epoll
     * 3.客户端超时
     * 4.事件存储结构？ 
     */

    if (_DEBUG)
    {
        printf("start accepting... \n");
    }

    int set_res = Set_nonblock(listenfd);
    if (set_res < 0)
    {
        printf("Set listenfd nonblock error \n");
        return _ERRCODE;
    }

    struct epoll_event wait_events[ MAX_EVENT_NUMBER ];
    int epollfd = epoll_create( EP_CREATE_SIZE ); //1? TODO:test
    Add_fd_epollin( epollfd, listenfd, ENABLE_ET );

    while( 1 )
    {
        int ret = epoll_wait( epollfd, wait_events, MAX_EVENT_NUMBER, TIMEOUT );
        if ( ret < 0 )
        {
            printf( "epoll wait error \n" );
            break;
        }
        else if ( ret == 0 )
        {
            printf( "timeout \n" );
            break;
        }
        

        for(int i=0; i<ret; ++i)
        {
            if(wait_events[i].data.fd == listenfd)
            {
                if (_DEBUG)
                {
                    printf("start accepting... \n");
                }
                
                struct sockaddr clientadd;
                socklen_t clientlen = sizeof(clientadd);
                
                //等待客户端连接，返回连接描述符
                connfd = accept(listenfd, &clientadd, &clientlen);
                if (connfd < 0)
                {
                    printf("Accept error \n");
                    return _ERRCODE;
                }
                else if (_DEBUG)
                {
                    printf("Accepted success, from %d \n", clientadd.sa_data[14]);
                }

                //将已清空的监听描述符重新设置事件类型，放入epoll符号表中
                // Mod_fd_epollin( epollfd, listenfd, ENABLE_ET );

                //将连接描述符设置为非阻塞
                int set_res = Set_nonblock(connfd);
                if (set_res < 0)
                {
                    printf("Set connfd EPOLLIN nonblock error \n");
                    return _ERRCODE;
                }

                //将已连接描述符加入epoll表中，监听可读事件，等待客户端发来的消息
                Add_fd_epollin( epollfd, connfd, ENABLE_ET );
            }
            else if (wait_events[i].events & EPOLLIN)
            {
                if (_DEBUG)
                {
                    printf("EPOLLIN \n");
                }

                //处理请求
                Handle_request(connfd);
                Mod_fd_epollout(epollfd, connfd, ENABLE_ET);
            }
            else if (wait_events[i].events & EPOLLOUT)
            {
                if (_DEBUG)
                {
                    printf("EPOLLOUT \n");
                }

                char buf[1024];
                sprintf(buf, "HTTP/1.0 200 OK\r\n");    
                sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
                sprintf(buf, "%sConnection: close\r\n\r\n", buf);
                write(connfd, buf, strlen(buf));       
                printf("Response headers:\n");
                printf("%s", buf);

                //关闭连接
                close(connfd);
            }
            else
            {
                printf("Unknown error，wait_events[i].data.fd：%d \n", wait_events[i].data.fd);
                break;
            }
            
        
        }
    
        // lt( events, ret, epollfd, listenfd );
        //et( events, ret, epollfd, listenfd );
    }
    
    // struct sockaddr clientadd;
    // socklen_t clientlen = sizeof(clientadd);
    
    // //等待客户端连接，返回连接描述符
    // int connfd = accept(listenfd, &clientadd, &clientlen);
    // if (connfd < 0)
    // {
    //     printf("Accept error \n");
    //     return _ERRCODE;
    // }
    // else if (_DEBUG)
    // {
    //     printf("Accepted connection from %s \n", clientadd.sa_data);
    // }


    // while (1)
    // {
    //     if (_DEBUG)
    //     {
    //         printf("start accepting... \n");
    //     }
        
    //     struct sockaddr clientadd;
    //     socklen_t clientlen = sizeof(clientadd);
        
    //     //等待客户端连接，返回连接描述符
    //     int connfd = accept(listenfd, &clientadd, &clientlen);
    //     if (connfd < 0)
    //     {
    //         printf("Accept error \n");
    //         return _ERRCODE;
    //     }
    //     else if (_DEBUG)
    //     {
    //         printf("Accepted connection from %s \n", clientadd.sa_data);
    //     }

    //     //处理请求
    //     Handle_request(connfd);
    //     //关闭连接
    //     close(connfd);
    // }

    //关闭连接
    // close(connfd);
    return _SUCCODE;
}