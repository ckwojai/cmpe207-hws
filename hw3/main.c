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
   char buf[LINELEN+1] = {0};
   time_t now;
   int sockfd = connectTCP(host, service);

   /* while ((nread = read(sockfd, buf, sizeof(time_t))) > 0); */
   /* while ((nread = read(sockfd, (char*) &now, sizeof(now))) > 0); */
   read(sockfd, &now, sizeof(now));
   /* time_t now = (time_t) buf; */
   /* printf("%s", ctime((time_t*) buf)); */
   now = ntohl((u_long)now); /* put in host byte order */
   now -= UNIXEPOCH; /* convert UCT to UNIX epoch */
   printf("%s", ctime(&now));
   close(sockfd);
}

int main() {
   /* const char* host = "ut1-wwv.nist.gov"; */
   const char* host = "ut1-wwv.nist.gov";
   const char* service = "time";
   TCPdaytime(host, service);
   return 0;
}
