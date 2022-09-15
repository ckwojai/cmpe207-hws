#include "active.h"
#include <unistd.h> // close()
#include <string.h> // strlen()
#include <stdio.h> // printf()

#define LINELEN 128


void TCPdaytime(const char* host, const char* service) {
   int nread;
   char buf[LINELEN+1];
   int sockfd = connectTCP(host, service);
   const char* msg = "what time is it?";
   if (write(sockfd, msg, strlen(msg)) < 0) {
      errexit("write to %s of %s gone wrong", service, host);
   }

   while ((nread = read(sockfd, buf, sizeof(buf))) > 0);
   printf("%s\n", buf);
   close(sockfd);
}

int main() {
   const char* host = "ut1-wwv.nist.gov";
   const char* service = "daytime";
   TCPdaytime(host, service);
   return 0;
}
