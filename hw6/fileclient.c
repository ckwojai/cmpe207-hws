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
   char buf[BUFSIZE];

   for (int i=0; i < 5; i++) {
      // establish a new connection
      int sockfd = connectTCP(host, service);
      char filename[20];
      snprintf(filename, 20, "%d_client.txt", i+1);
      int fd = open(filename, O_RDONLY);
      int filesize = lseek(fd, 0, SEEK_END);
      lseek(fd, 0, SEEK_SET);
      read(fd, &buf, filesize);
      printf("Sending %s to server......", filename);
      write(sockfd, &buf, filesize);
      printf("Done\n");
      // reset buf to clear junk
      memset(buf, '\0', strlen(buf));
      close(sockfd);
      // since the filename is determined by the order of request received from the server side
      // a little delay make sure client request 1-5 is received in order from the server side
      // otherwise, 12345 can be received as 13245 from the server side, and diff will fail
      sleep(1);
   }
   return 0;
}
