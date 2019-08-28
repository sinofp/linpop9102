#include <arpa/inet.h>
#include <mysql/mysql.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int clientfd[1024], i;

int userLogin(char *buf) {
  // 2|andy|1234
  char uName[32] = {0};
  char pWord[32] = {0};

  MYSQL mysql;
  mysql_init(&mysql);
  mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "chatroom", 0, NULL,
                     0);
  MYSQL_RES *result;
  MYSQL_ROW row;

  sscanf(buf + 2, "%[^|]|%s", uName, pWord);
  char sqlStr[1024] = {0};
  sprintf(sqlStr, "select password from user where username = '%s'", uName);
  puts(sqlStr);
  if (0 != mysql_query(&mysql, sqlStr)) {
    printf("%s\n", mysql_error(&mysql));
    return -1;
  }
  result = mysql_store_result(&mysql);
  if ((row = mysql_fetch_row(result))) {
    if (strcmp(row[0], pWord) == 0) {
      printf("登陆成功\n");
    } else {
      printf("wrong password\n");
      return -1;
    }
  } else {
    printf("unknown username");
    return -1;
  }
  return 0;
}

void *thread_recv(void *arg) {
  int i = *(int *)arg;
  int confd = clientfd[i - 1];
  char buf[1024];
  while (1) {
    bzero(buf, sizeof(buf));
    if (0 == recv(confd, buf, sizeof(buf), 0)) {
      clientfd[i] = 0;
      // 在clientfd里把自己的置0
      pthread_exit(NULL);
    }
    printf("recv: %s\n", buf);
    switch (buf[0]) {
    case '1':
      // zhu ce
      break;
    case '2':
      userLogin(buf);
      break;
    }
  }
}
void *thread_send(void *arg) {
  int i = *(int *)arg;
  int confd = clientfd[i - 1];
  char buf[1024];
  while (1) {
    if (0 == clientfd[i - 1]) {
      pthread_exit(NULL);
      // thread_recv里知道客户端下线了
    }
    scanf("%s", &buf);
    send(confd, buf, sizeof(buf), 0);
  }
}
int main() {
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
  /* bind(lisfd, (struct sockaddr*) &myaddr, sizeof(myaddr))
   * //给描述符绑定地址（ip、端口） */
  if (0 != bind(lisfd, (struct sockaddr *)&myaddr, sizeof(myaddr))) {
    printf("bind error\n");
    return -1;
  }
  listen(lisfd, 10);
  while (1) {
    struct sockaddr_in cliaddr;
    socklen_t socklen = sizeof(cliaddr);

    int confd = accept(lisfd, (struct sockaddr *)&cliaddr, &socklen);
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
      pthread_create(&tid, NULL, thread_send, &i);
      puts("new recv, send thread created.");
    }
  }
  return 0;
}
