#include "passive.h"

#include <sys/types.h> // u_long
#include <unistd.h> // close()
#include <string.h> // strlen()
#include <stdio.h> // printf()
#include <time.h> // time()
#include <arpa/inet.h> // ntohl()
#include <errno.h> // strerror(), errno

#define LINELEN 128
#define UNIXEPOCH 2208988800 /* UNIX epoch, in UCT secs */

int main() {
   const char* service = "time";
   char buf[1];
   time_t now;
   struct sockaddr_in fsin;
   socklen_t alen;
   int sockfd = passiveUDP(service);
   while (1) {
      alen = sizeof(fsin);
      if (recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&fsin, &alen) < 0) {
         errexit("recvfrom: %s\n", strerror(errno));
      }
      (void) time(&now);
      now = htonl((u_long)(now + UNIXEPOCH));
      (void) sendto(sockfd, (char *)&now, sizeof(now), 0, (struct sockaddr*)&fsin, sizeof(fsin));
   }
}
