#define _GNU_SOURCE
#define PEE(ERR_MSG) \
    perror(ERR_MSG); \
    exit(EXIT_FAILURE)

#include "database.h"
#include "helper.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_EVENTS 200

Message message;

MYSQL mysql;
MYSQL_RES* result;
MYSQL_ROW row;

struct epoll_event ev, events[MAX_EVENTS];
int listen_sock, conn_sock, nfds, epollfd;

int socket_bind_listen(int port)
{
    struct sockaddr_in myaddr;
    memset(&myaddr, 0, sizeof myaddr);
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = inet_addr("172.20.10.12");
    myaddr.sin_port = htons(port);

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (0 == sfd) {
        PEE("socket");
    }
    /* set non blocking */
    if (-1 == fcntl(sfd, F_SETFL, fcntl(sfd, F_GETFL) | O_NONBLOCK)) {
        PEE("fcntl");
    }
    if (0 != bind(sfd, (struct sockaddr*)&myaddr, sizeof myaddr)) {
        PEE("bind");
    }
    listen(sfd, 10);

    return sfd;
}

void reg(int fd)
{
    db_register(&message); //插入数据库
    message.msgType = REPLY;
    message.msgRet = SUCCESS;
    printf("\033[42;31m---send:\033[0m\n");
    printf("content:%s\ntype:%d\nfrom:%s\nto:%s\n", message.content, message.msgType, message.sendName, message.recvName);
    send(fd, &message, sizeof(message), 0);
}

void login(int fd)
{
    // 首先遍历在线列表，如果当前用户已在线，直接返回错误信息；
    if (user_already_online()) {
        message.msgType = REPLY;
        message.msgRet = FAILED;
        strcpy(message.content, "此用户已在线，请殴打冒用账号人再重试");
        printf("\033[42;31m---send:\033[0m\n");
        printf("content:%s\ntype:%d\nfrom:%s\nto:%s\n", message.content, message.msgType, message.sendName, message.recvName);
        send(fd, &message, sizeof(message), 0);
        return;
    }
    char secret[70] = "123"; // sha256后应该只有64位
    //先strcpy到secret，再调 查的函数，他的真密码会写在message.content里
    strcpy(secret, message.content);

    db_login(&message);
    // 根据用户名，得到哈希过的密码，对比和message.content一不一致
    // 一致：插入，回复成功，回复好友信息，回复当前在线人（遍历user——map），
    // 给所有人发一遍你上线了
    // 不一致：回复不成功

    if (0 == strlen(message.content) || 0 != strcmp(message.content, secret)) {
        //没有此用户，会给密码置空（memset）
        message.msgType = REPLY;
        message.msgRet = FAILED;
        strcpy(message.content, "账号或密码错误");
        printf("\033[42;31m---send:\033[0m\n");
        printf("content:%s\ntype:%d\nfrom:%s\nto:%s\n", message.content, message.msgType, message.sendName, message.recvName);
        send(fd, &message, sizeof(message), 0);
    } else {
        //        broadcast_loginout();

        message.msgRet = SUCCESS;
        message.msgType = REPLY;
        printf("\033[42;31m---send:\033[0m\n");
        printf("content:%s\ntype:%d\nfrom:%s\nto:%s\n", message.content, message.msgType, message.sendName, message.recvName);
        send(fd, &message, sizeof(message), 0);
        usleep(100);

        message.msgType = VIEW_RECORDS;
        db_list(&message); //得到好友列表，存在message.content里：好友名字，好友类型|好友名字，好友类型|...
        printf("\033[42;31m---send:\033[0m\n");
        printf("content:%s\ntype:%d\nfrom:%s\nto:%s\n", message.content, message.msgType, message.sendName, message.recvName);
        send(fd, &message, sizeof(message), 0);

        // 睡一觉，否则发太快，客户端socket.readyRead只触发一次
        usleep(100);

        message.msgType = VIEW_USER_LIST;
        //得到在线用户列表，存在message.content里：好友名字|好友名字|...
        send_current_online(fd);

        insert_user_fd(message.sendName, fd);
    }
}

void logout()
{
    delete_user_fd(message.sendName);
    broadcast_loginout();
}

// message.msgType: ADD_FRIEND / REMOVE_FRIEND, 这应该是客户端发来就写好的
void add_remove_friend(int fd)
{
    int wrong = 0;
    switch (message.msgType) {
    case ADD_FRIEND:
        wrong = db_addf(&message);
        break;
    case DELETE_FRIEND:
        wrong = db_delf(&message);
        break;
    }
    //不需要问对面同不同意，直接更新数据库，然后向被加的人发信息更新好友列表
    //todo if !wrong:
    printf("\033[42;31m---send:\033[0m\n");
    printf("content:%s\ntype:%d\nfrom:%s\nto:%s\n", message.content, message.msgType, message.sendName, message.recvName);
    send(fd, &message, sizeof(message), 0);

    fd = locate_user_fd(message.recvName);
    printf("\033[42;31m---send:\033[0m\n");
    printf("content:%s\ntype:%d\nfrom:%s\nto:%s\n", message.content, message.msgType, message.sendName, message.recvName);
    send(fd, &message, sizeof(message), 0);
}

void mirror()
{
    int fd = locate_user_fd(message.recvName);
    printf("\033[42;31m---send:\033[0m\n");
    printf("content:%s\ntype:%d\nfrom:%s\nto:%s\n", message.content, message.msgType, message.sendName, message.recvName);
    send(fd, &message, sizeof(message), 0);
}

void alter_friend()
{
    db_mvf(&message);
}

void echo(int confd)
{

    int recv_len = 0, offset = 0;
    while (1) {
        recv_len = recv(confd, (&message) + offset, sizeof message, 0);
        printf("recieved %d\t", recv_len);
        if (-1 == recv_len) {
            puts("recieved -1");
            return;
        }
        offset += recv_len;
        if (0 == recv_len || (sizeof message) == offset) {
            // 读完了
            break;
        }
    }

    printf("\033[46;31m---recv:\033[0m\n");
    printf("content:%s\ntype:%d\nfrom:%s\nto:%s\n", message.content, message.msgType, message.sendName, message.recvName);

    switch (message.msgType) {
    case REGISTER:
        reg(confd);
        break;
    case LOGIN:
        login(confd);
        break;
    case FL:
    case FL_CONTENT:
    case PERSONAL_CHAT:
        mirror();
        break;
    case ADD_FRIEND:
    case DELETE_FRIEND:
        add_remove_friend(confd);
        break;
    case EXIT:
        logout();
        break;
    case MOVE_FRIEND:
        alter_friend();
        break;
    case VIEW_USER_LIST:
        send_current_online(confd);
        break;
    }
}

int main()
{
    mysql_init(&mysql);
    if (mysql_real_connect(&mysql, "localhost", "root", NULL, "chatroom", 0, NULL, 0) == NULL) {
        printf("%s\n", mysql_error(&mysql));
        return -1;
    }

    listen_sock = socket_bind_listen(1234);

    epollfd = epoll_create1(0);
    if (epollfd == -1) {
        PEE("epoll_create1");
    }

    ev.events = EPOLLIN;
    ev.data.fd = listen_sock;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
        PEE("epoll_ctl: listen_sock");
    }

    for (;;) {
        nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            PEE("epoll_wait");
        }

        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == listen_sock) {
                //如果有新的连接来了
                struct sockaddr_in addr;
                socklen_t addrlen = sizeof addr;

                /* https://stackoverflow.com/a/22339017 */
                conn_sock = accept4(listen_sock,
                    (struct sockaddr*)&addr, &addrlen, SOCK_NONBLOCK);
                if (conn_sock == -1) {
                    PEE("accept");
                }

                printf("\033[46;31mclient: ip=%s\tport=%d\033[0m\n", inet_ntoa(addr.sin_addr),
                    ntohs(addr.sin_port));

                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_sock;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock,
                        &ev)
                    == -1) {
                    PEE("epoll_ctl: conn_sock");
                }
            } else {
                /* do_use_fd(events[n].data.fd); */
                echo(events[n].data.fd);
                //https://github.com/shineyr/Socket/blob/master/epoll_socket/server.c
                //todo 需要修改epoll event么？
            }
        }
    }
}
