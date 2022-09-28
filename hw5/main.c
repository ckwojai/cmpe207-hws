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
   time_t now; // holds up to 8 bytes
   int sockfd = connectTCP(host, service);

   // Expecting to only receive 4 bytes integer representation of time from server
   nread = read(sockfd, &now, sizeof(now)); // doesn't have to be a char*, any void* pointer would work
   // If more bytes coming, throw error
   nread = read(sockfd, buf, sizeof(buf));
   if (nread > 0) {
      errexit("Unexpected bytes from server. Expected only <=%d bytes. Received %d more.\n", sizeof(now), nread);
   }

   now = ntohl((u_long)now); /* put in host byte order */
   now -= UNIXEPOCH; /* convert UCT to UNIX epoch */
   printf("%s", ctime(&now));
   close(sockfd);
}

int main() {
   const char* host = "ut1-wwv.nist.gov";
   const char* service = "time"; // NOTE: NOT daytime
   TCPdaytime(host, service);
   return 0;
}
