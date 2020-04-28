#ifndef _EPOLL_H
#define _EPOLL_H

int Set_nonblock( int fd );
void Add_fd_epollin( int epollfd, int fd,  int enable_et );
void Add_fd_epollout( int epollfd, int fd, int enable_et );
void Mod_fd_epollin( int epollfd, int fd,  int enable_et );
void Mod_fd_epollout( int epollfd, int fd, int enable_et );
/**
 * fcntl 操作文件描述符，改变已打开文件的属性 
 * 此函数有两种参数传递方式：
 * 1.int fcntl(int fd, int cmd, int arg); 
 * fcntl函数功能依据cmd的值的不同而不同。参数对应功能如下：
 * （1）F_DUPFD / F_DUPFD_CLOEXEC
 * 复制由fd指向的文件描述符，调用成功后返回新的文件描述符，与旧的文件描述符共同指向同一个文件。
 * （2）F_GETFD / F_SETFD
 * 读取/设置文件描述符close-on-exec标志
 * （3）F_GETFL / F_SETFL
 * 读取/设置文件描述符状态标志
 * （4）F_GETOWN / F_SETOWN
 * 获取/设置异步IO所有权
 * 
 * 2.int fcntl(int fd, int cmd ,struct flock* lock); 
 * （5）F_GETLK / F_SETLK / F_SETLKW
 * 获取/设置记录锁
 * 文件记录锁是fcntl函数的主要功能。
 * 记录锁：实现只锁文件的某个部分，并且可以灵活的选择是阻塞方式还是立刻返回方式
 * 当fcntl用于管理文件记录锁的操作时，第三个参数指向一个struct flock *lock的结构体
 * struct flock  
 * {  
 *     short_l_type;    //锁的类型  
 *     short_l_whence;  //偏移量的起始位置：SEEK_SET,SEEK_CUR,SEEK_END  
 *     off_t_l_start;   //加锁的起始偏移
 *     off_t_l_len;     //上锁字节  
 *     pid_t_l_pid;     //锁的属主进程ID  
 * };  
 *  (6)F_SETLK
 * 此时fcntl函数用来设置或释放锁。当short_l_type为F_RDLCK为读锁，F_WDLCK为写锁，F_UNLCK为解锁。
 * 如果锁被其他进程占用，则返回-1;这种情况设的锁遇到锁被其他进程占用时，会立刻停止进程。
 *  (7)F_SETLKW
 * 此时也是给文件上锁，不同于F_SETLK的是，该上锁是阻塞方式。当希望设置的锁因为其他锁而被阻止设置时，该命令会等待相冲突的锁被释放。
 *  (8)F_GETLK
 * 第3个参数lock指向一个希望设置的锁的属性结构，如果锁能被设置，该命令并不真的设置锁，而是只修改lock的l_type为F_UNLCK,然后返回该结构体。如果存在一个或多个锁与希望设置的锁相互冲突，则fcntl返回其中的一个锁的flock结构。
 *  
 * 注意：在修改文件描述符标志或文件状态标志时，需先要获得现在的标志，然后再修改
 */

/**
 * int epoll_create(int size)
 * 该函数生成一个epoll专用的文件描述符,指定要访问的内核事件表。
 * 它其实是在内核申请一空间，用来存放你想关注的socket fd上是否发生以及发生了什么事件。
 * size就是你在这个epoll fd上能关注的最大socket fd数
 */

/**
 * int epoll_ctl(int epfd, int op, int fd, struct epoll_event* event); 
 * 该函数是epoll的事件注册函数，用来操作内核事件表
 * 第一个参数 epfd 是epoll_create()的返回值
 * 第二个参数 op 表示操作类型，有3种情况，用三个宏来表示：
 *   EPOLL_CTL_ADD：注册新的fd到epfd中；
 *   EPOLL_CTL_MOD：修改已经注册的fd的监听事件；
 *   EPOLL_CTL_DEL：从epfd中删除一个fd；
 * 第三个参数 fd 是需要操作的文件描述符
 * 第四个参数 event 是告诉内核需要监听什么事件
 *   epoll_event结构如下：
 * typedef union epoll_data {
 *      void *ptr;
 *      int fd;
 *      __uint32_t u32;
 *      __uint64_t u64;
 *  } epoll_data_t;
 * 
 *  struct epoll_event {
 *      __uint32_t events; // Epoll events 
 *      epoll_data_t data; // User data variable 
 *  };
 * 
 * events可以是以下几个宏的集合：
 *  EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
 *  EPOLLOUT：表示对应的文件描述符可以写；
 *  EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
 *  EPOLLERR：表示对应的文件描述符发生错误；
 *  EPOLLHUP：表示对应的文件描述符被挂断；
 *  EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
 *  EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里
 * 
 */

/**
 * 等待事件触发，当超过timeout还没有事件触发时，就超时。
 *    int epoll_wait(int epfd, struct epoll_event * events, intmaxevents, int timeout);
 *     等待事件的产生，类似于select()调用。参数events用来从内核得到事件的集合，maxevents告之内核这个events有多大(数组成员的个数)，这个maxevents的值不能大于创建epoll_create()时的size，参数timeout是超时时间（毫秒，0会立即返回，-1将不确定，也有说法说是永久阻塞）。
 *     该函数返回需要处理的事件数目，如返回0表示已超时。
 *     返回的事件集合在events数组中，数组中实际存放的成员个数是函数的返回值。返回0表示已经超时。
 * 函数声明:int epoll_wait(int epfd,struct epoll_event * events,int maxevents,int timeout)
 * 该函数用于轮询I/O事件的发生；
 * 参数：
 * epfd:由epoll_create 生成的epoll专用的文件描述符；
 * epoll_event:用于回传代处理事件的数组；
 * maxevents:每次能处理的事件数；
 * timeout:等待I/O事件发生的超时值(单位我也不太清楚)；-1相当于阻塞，0相当于非阻塞。一般用-1即可
 * 返回发生事件数。
 * epoll_wait运行的原理是
 * 等侍注册在epfd上的socket fd的事件的发生，如果发生则将发生的sokct fd和事件类型放入到events数组中。
 * 并 且将注册在epfd上的socket fd的事件类型给清空，所以如果下一个循环你还要关注这个socket fd的话，则需要用epoll_ctl(epfd,EPOLL_CTL_MOD,listenfd,&ev)来重新设置socket fd的事件类型。这时不用EPOLL_CTL_ADD,因为socket fd并未清空，只是事件类型清空。这一步非常重要。
 * 
 * 
 * 
 * 
 */




#endif