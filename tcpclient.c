#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int main() {
  char buf[1024] = {0};
  struct sockaddr_in sevaddr;

  memset(&sevaddr, 0, sizeof(sevaddr));
  sevaddr.sin_family = AF_INET;
  sevaddr.sin_port = htons(1234);
  sevaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  // ip port protocol

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == sockfd) {
    printf("create socket error\n");
    return -1;
  }

  if (0 == connect(sockfd, (struct sockaddr *)&sevaddr, sizeof(sevaddr))) {
    while (1) {
      scanf("%s", buf);
      send(sockfd, buf, strlen(buf), 0);

      memset(&buf, 0, sizeof(buf));
      recv(sockfd, buf, sizeof(buf), 0);

      printf("recv: %s\n", buf);
    }
  } else {
    printf("connect error\n");
    return -1;
  }
  return 0;
}
