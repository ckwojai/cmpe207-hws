#include "passive.h"

#include <stdlib.h> // exit
#include <sys/types.h> // u_long
#include <sys/signal.h> // signal
#include <sys/wait.h> // signal
#include <unistd.h> // close()
#include <string.h> // strlen()
#include <stdio.h> // printf()
#include <time.h> // time()
#include <arpa/inet.h> // ntohl()
#include <errno.h> // strerror(), errno

#define QLEN 5

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

void reaper(int sig) {
   int status;
   while (wait3(&status, WNOHANG, (struct rusage*)0)>=0);
}

int TCPechod(int fd) {
   char buf[BUFSIZ];
   int cc;
   while (cc = read(fd, buf, sizeof buf)) {
      if (cc < 0)
         errexit("echo read: %s\n", strerror(errno));
      printf("[%ld] Slave Socket %d receive request, sending \"%s\" back", (long)getpid(), fd, buf);
      if (write(fd, buf, cc) < 0)
         errexit("echo write:  %s\n", strerror(errno));
   }
   return 0;
}

int main() {
   struct sockaddr_in fsin;
   socklen_t alen;
   char* service = "echo";
   int msock;
   int ssock;

   msock = passiveTCP(service, QLEN);
   (void) signal(SIGCHLD, reaper);

   printf("Master Socket %d, Running in Process: %ld", msock, (long)getpid());

   while(1) {
     alen = sizeof(fsin);
     ssock = accept(msock, (struct sockaddr*) &fsin, &alen);
     if (ssock < 0) {
        if (errno == EINTR) {
           continue;
        }
        errexit("accept: %s\n", strerror(errno));
     }
     switch (fork()) {
         case 0: // child process
            printf("Slave Socket %d created, Running in Process: %ld", ssock, (long)getpid());
            (void) close(msock);
            exit(TCPechod(ssock));
         default: // parent process
            (void) close(ssock);
            break;
         case -1:
            errexit("fork: %s\n", strerror(errno));

     }
   }
}
