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
      write(sockfd, req, strlen(req));
      read(sockfd, res, strlen(res));
      printf("%s\n", res);
   }
   return 0;
}
