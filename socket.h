#ifndef _SOCKET_H
#define _SOCKET_H

int Open_listenfd(char *port);

    /**
     * 为了执行网络I/O，一个进程必须做的第一件事情就是创建一个socket函数
     * int socket(int family, int type, int protocol);
     * family：   表示协议族      AF_INET(IPv4协议)、AF_INET6(IPv6协议)、AF_LOCAL(Unix域协议)、AF_ROUTE(路由套接字)、AF_KEY(  密钥套接字)
     * type：     表示套接字类型   SOCK_STREAM(字节流套接字)、SOCK_DGRAM(数据报套接字)、SOCK_SEQPACKET(有序分组套接字)、SOCK_ROW(原始套接字)
     * protocol： 表示传输协议     IPPROTO_TCP(TCP传输协议)、IPPROTO_UDP(UDP传输协议)、IPPROTO_SCTP(SCTP传输协议)
     * 返回值：若成功返回非负描述符，若出错返回-1
     * 注意：并不是上面的type和protocol可以随意组合的，如SOCK_STREAM不可以跟IPPROTO_UDP组合。当protocol为0时，会自动选择type类型对应的默认协议。
     */
    // listenfd = socket(AF_INET, SOCK_STREAM, 0); 

    /**
     * struct sockaddr_in {
     *  uint8_t sin_len;         //结构体长度
     *  sa_family_t sin_family;  //AF_INET
     *  in_port_t sin_port;      //端口(uint16)
     *  struct in_addr sin_addr; //IPv4地址(uint32)
     *  char sin_zero[8];        //冗余，设置0即可
     * }
     */
    // struct sockaddr_in sa_in;                   /*声明一个变量，类型为协议地址类型*/
    // sa_in.sin_family = AF_INET;                 /*使用IPv4协议*/
    // sa_in.sin_port = htons(*port);              /*监听端口 htons:主机字节顺序转换为网络字节顺序(uint16) */
    // sa_in.sin_addr.s_addr = htonl(INADDR_ANY);  /*绑定本机IP，INADDR_ANY就是0.0.0.0，泛指本机，意思是只要端口对应，本机所有网卡的数据都能接收到 htonl:主机字节顺序转换为网络字节顺序(uint32)*/

    /**
     * bind函数把 协议地址 和 套接字描述符 关联起来
     * int bind(int sockfd, const struct sockaddr *myaddr, socklen_t addrlen)
     * sockfd   初始化的套接字
     * myaddr   协议地址
     * addrlen  协议地址长度
     * 若成功返回0 出错返回-1
     */
    // bind(listenfd ,(struct sockaddr *)&sa_in, sizeof(sa_in));

    /**
     * int listen(int sockfd, int backlog)
     * sockfd默认为一个主动套接字，也就是客户端所用的套接字描述符。
     * 当调用listen函数之后，它被转成一个被动套接字。
     * backlog  这个函数规定内核应该为相应套接字排队的最大连接数
     * 成功返回0 出错返回-1
     */
    // listen(listenfd, 20)

#endif


