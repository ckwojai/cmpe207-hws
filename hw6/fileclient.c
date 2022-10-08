#include "active.h"

#include <unistd.h> // close()
#include <string.h> // strlen()
#include <stdio.h> // printf()
#include <fcntl.h> // open(), O_WRONLY, O_CREAT

#define LINELEN 128
#define BUFSIZE 65536  // 64kiB = 1024 * 64

int main() {
   const char* host = "localhost";
   const char* service = "file";
   int sockfd = connectTCP(host, service);
   printf("Connected to %s service on %s :\n", service, host);
   char buf[BUFSIZE];
   char* filename = "1_client.txt";


   int fd = open(filename, O_RDONLY);
   int filesize = lseek(fd, 0, SEEK_END);
   lseek(fd, 0, SEEK_SET);
   read(fd, &buf, filesize);
   write(sockfd, &buf, filesize);
   close(sockfd);
   return 0;
}
