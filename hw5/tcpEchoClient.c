#include "active.h"

#include <unistd.h> // close()
#include <string.h> // strlen()
#include <stdio.h> // printf()

#define LINELEN 128

int main() {
   const char* host = "localhost";
   const char* service = "echo";
   int cc;
   int sockfd = connectTCP(host, service);
   printf("Connected to Echo Server:\n");
   char req[LINELEN];
   char res[LINELEN];
   while (1) {
      fgets(req, LINELEN, stdin);
      req[strcspn(req, "\n")] = 0; // remove trailing newline character
      write(sockfd, req, strlen(req));
      read(sockfd, res, strlen(res));
      /* while(cc = read(sockfd, res, sizeof res)); */
      printf("%s \n", res);
      memset(res, '\0', strlen(res));
   }
   close(sockfd);
   return 0;
}
