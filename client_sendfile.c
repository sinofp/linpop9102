#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define BUF_SIZE 1024
#define FILE_NAME_MAX 512
int main()
{

	
    char buf[BUF_SIZE] = { 0 };
    char file_name[FILE_NAME_MAX]={0};
    
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

    if (0 == connect(sockfd, (struct sockaddr*)&sevaddr, sizeof(sevaddr))) {

	    while(1){
	    	printf("输入要发送的文件名：\n");
      

                scanf("%s", file_name);
		bzero(buf,BUF_SIZE);
		strncpy(buf,file_name,BUF_SIZE>strlen(file_name)?strlen(file_name):BUF_SIZE);    
               printf("file_name:%s,buf:%s\n",file_name,buf); 
	       send(sockfd, buf, strlen(buf), 0);

		ssize_t length=0;
		 FILE *fp=fopen(file_name,"w");
                if(fp==NULL)
                {
                        printf("File:\t%s Can Not Open To Write\n", file_name);
                        return 1;

                
		}
	
            while((length = read(sockfd, buf, sizeof(buf)))&&length>=BUF_SIZE )   
	  { 
	  
	    if(fwrite(buf, sizeof(char), length, fp) != length)   
	    {   
	      printf("File:%s Write Failed\n", file_name);   
	      break;   
	    }   
	    bzero(buf, BUF_SIZE);   
	  
	  }
	     if(fwrite(buf, sizeof(char), length, fp) != length)
            {
              printf("File:%s Write Failed\n", file_name);
              break;
            }
            bzero(buf, BUF_SIZE);

	 printf("Receive File:%s From Server IP Successful!\n", file_name);
         fclose(fp);    
	    }
     
    } else {
        printf("connect error\n");
        return -1;
    }
    return 0;
}

