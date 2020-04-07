#ifndef _REQUEST_H
#define _REQUEST_H


void Handle_request(int connfd);

typedef struct RequestLine {
    char* path;
    char* method;
    char* version;
} RequestLine;

RequestLine* ReqLine;
#endif


/**
 *定义函数：int strcasecmp (const char *s1, const char *s2);
 *函数说明：strcasecmp()用来比较参数s1 和s2 字符串，比较时会自动忽略大小写的差异。
 *返回值：若参数s1 和s2 字符串相同则返回0。s1 长度大于s2 长度则返回大于0 的值，s1 长度若小于s2 长度则返回小于0 的值。
*/
// strcasecmp(method, "GET")


//读取请求行
// int read_request_line(int connd, char *buf, int len)
// {
//     int n;
//     int is_r, r_point = 0;
//     while (n = read(connd, buf, 1) != 0)
//     {
//         if (_DEBUG)
//         {
//             printf("%s \n", buf);
//         }
        
//         if(n == -1)
//         {
//             return _ENDCODE;
//         }

//         if(buf == "\r")
//         {
//             is_r = 1;
//             r_point = len;
//         }

//         if (buf == "\n" && is_r && len == (r_point - 1))
//         {
//             return _SUCCODE;
//         }
        
//         len++;
//         buf++;
//     }
// }