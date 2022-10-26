#include "active.h"

#include <unistd.h> // close()
#include <string.h> // strlen()
#include <stdio.h> // printf()
#include <fcntl.h> // open(), O_WRONLY, O_CREAT
#include <time.h> // ctime()
#include <arpa/inet.h> // ntohl()
#include <errno.h> // errono, strerror()

#define LINELEN 128
#define BUFSIZE 65536  // 64kiB = 1024 * 64
#define UNIXEPOCH 2208988800 /* UNIX epoch, in UCT secs */
#define MSG "what time is it?\n"

void UDPdaytime(const char* host, const char* service) {
   int nread;
   char buf[LINELEN+1] = {0};
   time_t now; // holds up to 8 bytes
   int sockfd = connectUDP(host, service);

   write(sockfd, MSG, strlen(MSG));

   // Expecting to only receive 4 bytes integer representation of time from server
   nread = read(sockfd, &now, sizeof(now)); // doesn't have to be a char*, any void* pointer would work
   if (nread < 0) {
     errexit("read failed: %s\n", strerror(errno));
   }

   now = ntohl((u_long)now); /* put in host byte order */
   now -= UNIXEPOCH; /* convert UCT to UNIX epoch */
   printf("%s", ctime(&now));
   close(sockfd);
}

int main() {
   const char* host = "localhost";
   const char* service = "time"; // NOTE: NOT daytime
   UDPdaytime(host, service);
   return 0;
}
