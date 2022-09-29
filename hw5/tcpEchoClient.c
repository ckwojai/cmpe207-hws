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
   printf("Connected to %s service on %s :\n", service, host);
   char req[LINELEN];
   char res[LINELEN];
   while (1) {
      fgets(req, LINELEN, stdin);
      req[strcspn(req, "\n")] = 0; // remove trailing newline character
      write(sockfd, req, strlen(req));
      n = read(sockfd, res, LINELEN); /*TODO: why this doesn't work: while((n = read(sockfd, res, LINELEN)) > 0) { */
      res[n] = '\0'; // make sure string is null-terminated
      printf("%s \n", res);
   }
   close(sockfd);
   return 0;
}
