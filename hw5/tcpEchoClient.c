#include "active.h"

#include <unistd.h> // close()
#include <string.h> // strlen()
#include <stdio.h> // printf()

#define LINELEN 128

int main() {
   const char* host = "localhost";
   const char* service = "echo";
   int n;
   int sockfd = connectTCP(host, service);
   printf("Connected to Echo Server:\n");
   char req[LINELEN];
   char res[LINELEN];
   while (1) {
      printf("Input again:\n");
      fgets(req, LINELEN, stdin);
      req[strcspn(req, "\n")] = 0; // remove trailing newline character
      write(sockfd, req, strlen(req));
      n = read(sockfd, res, LINELEN);
      res[n] = '\0';
      /* while((n = read(sockfd, res, LINELEN)) > 0) { */
      /*       res[n] = '\0'; */
      /* } */
      printf("%s \n", res);
      /* memset(res, '\0', strlen(res)); */
   }
   close(sockfd);
   return 0;
}
