#include <arpa/inet.h>
#include <mysql/mysql.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int clientfd[1024], i;

void* thread_recv(void* arg)
{
    int i = *(int*)arg;
    int confd = clientfd[i - 1];
    char buf[1024];
    while (1) {
        bzero(buf, sizeof(buf));
        if (0 == recv(confd, buf, sizeof(buf), 0)) {
            clientfd[i] = 0;
            // 在clientfd里把自己的置0
            pthread_exit(NULL);
        }
        printf("recv from client%d: %s\n", i, buf);
        int to_client;
        sscanf(buf, "%d|%s", &to_client, buf);

        to_client--;
        printf("check: to_client:%d: %s\n", to_client, buf);
        send(clientfd[to_client], buf, sizeof(buf), 0);
    }
}

int main()
{
    struct sockaddr_in myaddr;
    memset(&myaddr, 0, sizeof(myaddr));

    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    myaddr.sin_port = htons(1234);

    int lisfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lisfd == 0) {
        printf("socket() failed\n");
        return -1;
    }
    if (0 != bind(lisfd, (struct sockaddr*)&myaddr, sizeof(myaddr))) {
        printf("bind error\n");
        return -1;
    }
    listen(lisfd, 10);
    while (1) {
        struct sockaddr_in cliaddr;
        socklen_t socklen = sizeof(cliaddr);

        int confd = accept(lisfd, (struct sockaddr*)&cliaddr, &socklen);
        if (-1 == confd) {
            printf("accept failed\n");
            /* return -1; */
        } else {
            clientfd[i++ % 1024] = confd;
            // 应该判断哪个是空的再看
            printf("client: ip=%s\tport=%d\n", inet_ntoa(cliaddr.sin_addr),
                ntohs(cliaddr.sin_port));
            pthread_t tid;
            pthread_create(&tid, NULL, thread_recv, &i);
        }
    }
    return 0;
}
