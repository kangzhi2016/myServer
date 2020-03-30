#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include "request.h"
#include "bison.tab.h"


//调试模式开关 0=关 1=开
#define	_DEBUG	1
//以下三个码分别代表：成功、正常结束、失败
#define	_SUCCODE 0
#define	_ENDCODE 1
#define _ERRCODE (-1)
//数据接收时用到的buffer长度
#define BUFLEN 1024
#define STRLEN 1024

// typedef struct RequestLine {
//     char* uri;
//     char* method;
//     char* version;
// } RequestLine;

extern int yyparse();
extern int Request_parse(char *buf, int len);

static void send_response(int fd);
static int read_client_data(int connd, char *buf);
static void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg); 

void Handle_request(int connfd)
{
    //读取数据
    // int len = 0;
    // char buf[BUFLEN];
    char *buf, *method, *uri, *version;
    // char buf[BUFLEN], method[STRLEN], uri[STRLEN], version[STRLEN];

    buf = calloc(1, 1024);
    if(read_client_data(connfd, buf) != _SUCCODE)
    {
        printf("Data error, buf is %s", buf);
        return;
    }
    printf("buf read over, buf is %s", buf);
    
    //普通解析数据：
    method = calloc(1, 16);
    uri = calloc(1, 16);
    version = calloc(1, 16);
    // if(sscanf(buf, "%s %s %s", method, uri, version))
    // {
    //     printf("buf parse success. method:%s uri:%s version:%s \n", method, uri, version);
    // }
    // else
    // {
    //     printf("buf parse error... \n");
    // }
    
    //词法语法分析解析数据：
    extern struct RequestLine* ReqLine;
    Request_parse(buf, sizeof(buf));
    strcpy(method, ReqLine->method);
    strcpy(uri, ReqLine->uri);
    strcpy(version, ReqLine->version);
    printf("buf parse success. method:%s abs_path:%s version:%s\n", method, uri, version);

    //判断请求方法、判断请求协议
    if (strcasecmp(method, "get")) 
    {
        clienterror(connfd, method, "501", "Method not Implemented", "Tiny does not implement this method");
        return;
    }
    if (strcasecmp(version, "http/1.0") && strcasecmp(version, "http/1.1")) 
    {
        clienterror(connfd, version, "502", "http version not Implemented", "Tiny does not implement this version");
        return;
    }
        
    //解析请求路径
    //读取请求文件
    //构造返回结果
    if (strcasecmp(uri, "/") || strcasecmp(uri, "index.html") || strcasecmp(uri, "/index.html"))
    {
        send_response(connfd);
    }

    close(connfd);
    return;
}

/*
读取客户端数据
*/
static int read_client_data(int connd, char *buf)
{
    int n;
    int len = 0;
    int read_len = 1;

    while ((n = read(connd, buf, read_len)) == read_len)
    {
        if (_DEBUG)
        {
            printf("%s \n", buf);
        }

        len++;
        buf++;

        if (len > 4 && buf[-4] == '\r' && buf[-3] == '\n' && buf[-2] == '\r' && buf[-1] == '\n')
        {
            return _SUCCODE;
        }
    }

    if(n == -1)
    {
        return _ERRCODE;
    }else
    {
        return _ENDCODE;
    }
}

/**
 * 返回请求结果 
 */
static void send_response(int fd)
{
    char buf[STRLEN];
 
    sprintf(buf, "HTTP/1.0 200 OK\r\n");    
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    sprintf(buf, "%sConnection: close\r\n\r\n", buf);
    write(fd, buf, strlen(buf));       
    printf("Response headers:\n");
    printf("%s", buf);
}

static void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg) 
{
    char buf[STRLEN], body[STRLEN];

    /* Build the HTTP response body */
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    write(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    write(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    write(fd, buf, strlen(buf));
    write(fd, body, strlen(body));
}