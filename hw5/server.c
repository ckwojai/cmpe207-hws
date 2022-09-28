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

char *get_ip_str(const struct sockaddr *sa, char *s, size_t maxlen)
{
    switch(sa->sa_family) {
        case AF_INET:
            inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr),
                    s, maxlen);
            break;

        case AF_INET6:
            inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr),
                    s, maxlen);
            break;

        default:
            strncpy(s, "Unknown AF", maxlen);
            return NULL;
    }

    return s;
}

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

      // Convert fsin to address in string
      char clientAddr[100];
      (void) get_ip_str((struct sockaddr*) &fsin, clientAddr, 100);

      (void) time(&now);
      printf("Received connection from %s. Sending back time: %s...", clientAddr, ctime(&now));
      now = htonl((u_long)(now + UNIXEPOCH));

      (void) sendto(sockfd, (char *)&now, sizeof(now), 0, (struct sockaddr*)&fsin, sizeof(fsin));
      printf("...SUCCESS\n");
   }
}
