#include "active.h"

#include <sys/types.h> // u_long
#include <unistd.h> // close()
#include <string.h> // strlen()
#include <stdio.h> // printf()
#include <time.h> // ctime()
#include <arpa/inet.h> // ntohl()

#define LINELEN 128
#define UNIXEPOCH 2208988800 /* UNIX epoch, in UCT secs */

void TCPdaytime(const char* host, const char* service) {
   int nread;
   time_t now;
   char buf[LINELEN+1] = {0};
   int sockfd = connectTCP(host, service);
   const char* msg = "what time is it?\n";
   if (write(sockfd, msg, strlen(msg)) < 0) {
      errexit("write to %s of %s gone wrong", service, host);
   }

   /* while ((nread = read(sockfd, buf, sizeof(buf))) > 0); */
   while ((nread = read(sockfd, (char*) &now, 1)) > 0);
   /* read(sockfd, (char*) &now, sizeof(now)); */

   /* printf("%s", buf); */
   now = ntohl((u_long)now); /* put in host byte order */
   now -= UNIXEPOCH; /* convert UCT to UNIX epoch */
   printf("%s", ctime(&now));
   close(sockfd);
}

int main() {
   /* const char* host = "ut1-wwv.nist.gov"; */
   const char* host = "ut1-wwv.nist.gov";
   const char* service = "daytime";
   TCPdaytime(host, service);
   return 0;
}
