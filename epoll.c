#include <stdio.h>
#include <fcntl.h>
#include <sys/epoll.h>

#include "epoll.h"

//调试模式开关 0=关 1=开
#define	_DEBUG	1
//以下三个码分别代表：成功、正常结束、失败
#define	_SUCCODE 0
#define	_ENDCODE 1
#define _ERRCODE (-1)

static void ctl_fd( int epollfd, int fd, int my_event, int op, int enable_et );

int Set_nonblock( int fd )
{
    int old_option, new_option;
    if (old_option = fcntl( fd, F_GETFL, 0 ) < 0)
    {
        printf("F_GETFL error \n");
        return _ERRCODE;
    }    

    new_option = old_option | O_NONBLOCK;

    if (fcntl( fd, F_SETFL, new_option ) < 0)
    {
        printf("F_SETFL error \n");
        return _ERRCODE;
    }

    return _SUCCODE;
}

void Add_fd_epollin( int epollfd, int fd, int enable_et )
{
    ctl_fd(epollfd, fd, EPOLLIN, EPOLL_CTL_ADD,  enable_et);
}
void Add_fd_epollout( int epollfd, int fd, int enable_et )
{
    ctl_fd(epollfd, fd, EPOLLOUT, EPOLL_CTL_ADD,  enable_et);
}
void Mod_fd_epollin( int epollfd, int fd, int enable_et )
{
    ctl_fd(epollfd, fd, EPOLLIN, EPOLL_CTL_MOD,  enable_et);
}
void Mod_fd_epollout( int epollfd, int fd, int enable_et )
{
    ctl_fd(epollfd, fd, EPOLLOUT, EPOLL_CTL_MOD, enable_et);
}
static void ctl_fd( int epollfd, int fd, int my_event, int op,int enable_et )
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = my_event;
    if( enable_et )
    {
        event.events |= EPOLLET;
    }
    epoll_ctl( epollfd, op, fd, &event );
}

