#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>


//调试模式开关 0=关 1=开
#define	_DEBUG	1
//以下三个码分别代表：成功、正常结束、失败
#define	_SUCCODE 0
#define	_ENDCODE 1
#define _ERRCODE (-1)
//允许排队等待链接的请求数
#define backlog 10

int Open_listenfd(char *port)
{
    int listenfd;

    //生成监听描述符
    listenfd = socket(AF_INET, SOCK_STREAM, 0); 
    if(listenfd < 0){
        printf("socket error  \n");
        return _ERRCODE;
    }
    else
    {
        printf("socket success. \n");
    }
    
    //定义通用地址，并赋初始值
    struct sockaddr_in sa_in;                   /*声明一个变量，类型为协议地址类型*/
    sa_in.sin_family = AF_INET;                 /*使用IPv4协议*/
    // sa_in.sin_port = htons(*port);           /*错误用法，导致实际监听端口跟期望不一致*/
    sa_in.sin_port = htons(atoi(port));         /*监听端口 htons:主机字节顺序转换为网络字节顺序(uint16) */
    sa_in.sin_addr.s_addr = htonl(INADDR_ANY);  /*绑定本机IP，INADDR_ANY就是0.0.0.0，泛指本机，意思是只要端口对应，本机所有网卡的数据都能接收到 htonl:主机字节顺序转换为网络字节顺序(uint32)*/

    //绑定对应端口
    if(bind(listenfd ,(struct sockaddr *)&sa_in, sizeof(sa_in)) < 0){
        close(listenfd);
        printf("bind error \n");
        return _ERRCODE;
    }
    else
    {
        printf("bind success. \n");
    }

    //监听端口
    if(listen(listenfd, backlog) < 0){
        close(listenfd);
        printf("listen error \n");
        return _ERRCODE;
    }
    else
    {
        printf("listen success. \n");
    }

    return listenfd;
}