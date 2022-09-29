#include "active.h"

#include <unistd.h> // close()
#include <string.h> // strlen()
#include <stdio.h> // printf()

#define LINELEN 128

int main() {
   const char* host = "localhost";
   const char* service = "echo";
   int sockfd = connectTCP(host, service);
   printf("Connected to Echo Server:\n");
   char req[LINELEN];
   char res[LINELEN];
   while (1) {
      fgets(req, LINELEN, stdin);
      req[strcspn(req, "\n")] = 0; // remove trailing newline character
      write(sockfd, req, strlen(req));
      while(read(sockfd, res, sizeof res) > 0);
      printf("%s\n", res);
   }
   close(sockfd);
   return 0;
}
