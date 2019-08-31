#include "msg.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int sockfd;
char buf[BUF_SIZE];
char my_name[20];
Message message;

void login()
{
    //    "0|<name>|passwd to login"
    //    hash passwd first
    memset(&message, 0, sizeof(message));
    sscanf(buf + 2, "%[^|]|%s", my_name, message.content);
    strcpy(message.sendName, my_name);
    message.msgType = LOGIN;
    send(sockfd, &message, sizeof(message), 0);
}

void send1()
{
    //    1|<recv_name>|<msg> to send msg
    memset(&message, 0, sizeof(message));
    sscanf(buf + 2, "%[^|]|%s", message.recvName, message.content);
    // sendName 在登录时设置了，再也不改
    strcpy(message.sendName, my_name);
    message.msgType = PERSONAL_CHAT;
    send(sockfd, &message, sizeof(message), 0);
}

int main()
{
    struct sockaddr_in sevaddr;

    memset(&sevaddr, 0, sizeof(sevaddr));
    sevaddr.sin_family = AF_INET;
    sevaddr.sin_port = htons(1234);
    sevaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // ip port protocol

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sockfd) {
        printf("create socket error\n");
        return -1;
    }

    puts("0|<name>|passwd to login");
    puts("1|<recv_name>|<msg> to send msg");

    if (0 == connect(sockfd, (struct sockaddr*)&sevaddr, sizeof(sevaddr))) {
        pid_t pid = fork();

        if (pid > 0) {
            while (1) {
                //发送进程
                scanf("%s", buf);
                switch (buf[0]) {
                case '0':
                    login(buf);
                    break;
                case '1':
                    send1(buf);
                    break;
                }
            }
        } else if (0 == pid) {
            while (1) {
                //接收进程
                memset(&message, 0, sizeof(message));
                recv(sockfd, &message, sizeof(message), 0);
                // buf只用来发送时解包
                //                printf("---recv:\n");
                //                write(STDOUT_FILENO, buf, sizeof(buf));
                switch (message.msgType) {
                case REPLY:
                    if (SUCCESS == message.msgRet) {
                        puts("\033[42;30mLOGIN SUCCESS~!\033[0m");
                    } else {
                        puts("\033[41;30mNOPE!\033[0m");
                    }
                    break;
                case PERSONAL_CHAT:
                    printf("%s send you:\t", message.sendName);
                    // 把这个message.content放在上面的printf里会整句都打印不出来，不知道为什么
                    puts(message.content);
                    break;
                }
            }
        } else {
            printf("fork error");
        }
    } else {
        printf("connect error\n");
        return -1;
    }
    return 0;
}
