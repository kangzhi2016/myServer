#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#include "socket.h"
#include "request.h"

//调试模式开关 0=关 1=开
#define	_DEBUG	1
//以下三个码分别代表：成功、正常结束、失败
#define	_SUCCODE 0
#define	_ENDCODE 1
#define _ERRCODE (-1)


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
    
    int listenfd;

    //创建 socket，返回监听描述符
    listenfd = Open_listenfd(argv[1]);
    if (listenfd < 0)
    {
        printf("Open listenfd error \n");
        return _ERRCODE;
    }

    while (1)
    {
        if (_DEBUG)
        {
            printf("start accepting... \n");
        }
        
        struct sockaddr clientadd;
        socklen_t clientlen = sizeof(clientadd);
        
        //等待客户端连接，返回连接描述符
        int connfd = accept(listenfd, &clientadd, &clientlen);
        if (connfd < 0)
        {
            printf("Accept error \n");
            return _ERRCODE;
        }
        else if (_DEBUG)
        {
            printf("Accepted connection from %s \n", clientadd.sa_data);
        }

        //处理请求
        Handle_request(connfd);
        //关闭连接
        close(connfd);
    }

    return _SUCCODE;
}