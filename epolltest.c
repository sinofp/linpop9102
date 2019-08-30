#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX_EVENTS 10

struct epoll_event ev, events[MAX_EVENTS];
int listen_sock, conn_sock, nfds, epollfd;

int socket_bind_listen(int port)
{
    struct sockaddr_in myaddr;
    memset(&myaddr, 0, sizeof myaddr);
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    myaddr.sin_port = htons(port);

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (0 == sfd) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }
    if (0 != bind(sfd, (struct sockaddr*)&myaddr, sizeof myaddr)) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    listen(sfd, 10);

    return sfd;
}

void echo(int confd)
{
    char buf[1024] = { 0 };
    if (0 == recv(confd, buf, sizeof buf, 0)) {
        puts("recieved 0 from conn");
        return;
    }

    printf("recv %s\n", buf);
    send(confd, buf, sizeof buf, 0);
}

int main()
{
    int listen_sock = socket_bind_listen(1234);

    epollfd = epoll_create1(0);
    if (epollfd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = listen_sock;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
        perror("epoll_ctl: listen_sock");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == listen_sock) {
                //如果有新的连接来了
                struct sockaddr_in addr;
                socklen_t addrlen = sizeof addr;

                conn_sock = accept(listen_sock,
                    (struct sockaddr*)&addr, &addrlen);
                if (conn_sock == -1) {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }

                printf("client: ip=%s\tport=%d\n", inet_ntoa(addr.sin_addr),
                    ntohs(addr.sin_port));
                /* ??????? */
                /* setnonblocking(conn_sock); */
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_sock;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,
                        &ev)
                    == -1) {
                    perror("epoll_ctl: conn_sock");
                    exit(EXIT_FAILURE);
                }
            } else {
                /* do_use_fd(events[n].data.fd); */
                echo(events[n].data.fd);
            }
        }
    }
}
