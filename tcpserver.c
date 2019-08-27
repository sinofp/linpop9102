#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  char buf[1024] = {0};
  struct sockaddr_in myaddr;
  struct sockaddr_in cliaddr;
  memset(&myaddr, 0, sizeof(myaddr));

  myaddr.sin_family = AF_INET;
  myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  myaddr.sin_port = htons(1234);

  int lisfd = socket(AF_INET, SOCK_STREAM, 0);
  if (lisfd == 0) {
    printf("socket() failed\n");
    return -1;
  }
  socklen_t socklen = sizeof(cliaddr);
  /* bind(lisfd, (struct sockaddr*) &myaddr, sizeof(myaddr))
   * //给描述符绑定地址（ip、端口） */
  if (0 != bind(lisfd, (struct sockaddr *)&myaddr, sizeof(myaddr))) {
    printf("bind error\n");
    return -1;
  }
  listen(lisfd, 10);
  int confd = accept(lisfd, (struct sockaddr *)&cliaddr, &socklen);
  if (-1 == confd) {
    printf("accept failed\n");
    return -1;
  }
  printf("client: ip=%s\tport=%d\n", inet_ntoa(cliaddr.sin_addr),
         ntohs(cliaddr.sin_port));

  pid_t pid = fork();
  if (pid > 0) {
    while (1) {
      recv(confd, buf, sizeof(buf), 0);
      printf("recv: %s\n", buf);
      memset(&buf, 0, sizeof(buf));
    }
  } else if (0 == pid) {
    while (1) {
      scanf("%s", &buf);
      send(confd, buf, sizeof(buf), 0);
    }
  }
  return 0;
}
