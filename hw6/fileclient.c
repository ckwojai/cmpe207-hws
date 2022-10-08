#include "active.h"

#include <unistd.h> // close()
#include <string.h> // strlen()
#include <stdio.h> // printf()
#include <fcntl.h> // open(), O_WRONLY, O_CREAT

#define LINELEN 128
#define BUFSIZE 65536  // 64kiB = 1024 * 64

int main() {
   const char* host = "localhost";
   const char* service = "ftp";
   int sockfd = connectTCP(host, service);
   printf("Connected to %s service on %s :\n", service, host);
   char buf[BUFSIZE];

   for (int i=0; i < 5; i++) {
      char filename[20];
      snprintf(filename, 20, "%d_client.txt", i+1);
      int fd = open(filename, O_RDONLY);
      int filesize = lseek(fd, 0, SEEK_END);
      lseek(fd, 0, SEEK_SET);
      read(fd, &buf, filesize);
      printf("Sending %s to server.", filename);
      write(sockfd, &buf, filesize);
      // reset buf to clear junk
      memset(buf, '\0', strlen(buf));
   }

   close(sockfd);
   return 0;
}
