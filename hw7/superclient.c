#include "active.h"

#include <unistd.h> // close()
#include <string.h> // strlen()
#include <stdio.h> // printf()
#include <fcntl.h> // open(), O_WRONLY, O_CREAT
#include <time.h> // ctime()
#include <arpa/inet.h> // ntohl()
#include <errno.h> // errono, strerror()

#define LINELEN 512
#define BUFSIZE 65536  // 64kiB = 1024 * 64
#define UNIXEPOCH 2208988800 /* UNIX epoch, in UCT secs */
#define MSG "what time is it?\n"


void UDPtimec(const char* host) {
   char* service = "time";
   int nread;
   char buf[LINELEN+1] = {0};
   time_t now; // holds up to 8 bytes
   int sockfd = connectUDP(host, service);

   write(sockfd, MSG, strlen(MSG));

   // Expecting to only receive 4 bytes integer representation of time from server
   nread = read(sockfd, &now, sizeof(now)); // doesn't have to be a char*, any void* pointer would work

   now = ntohl((u_long)now); /* put in host byte order */
   now -= UNIXEPOCH; /* convert UCT to UNIX epoch */
   printf("%s", ctime(&now));
   close(sockfd);
}

void UDPdaytimec(const char* host) {
   char* service = "daytime";
   int nread;
   char buf[LINELEN+1] = {0};
   int sockfd = connectUDP(host, service);

   write(sockfd, MSG, strlen(MSG));

   nread = read(sockfd, buf, strlen(buf));
   printf("%s", buf);
   close(sockfd);
}

void UDPchargenc(const char* host) {
   char* service = "chargen";
   int nread;
   char buf[LINELEN+1] = {0};
   int sockfd = connectUDP(host, service);

   write(sockfd, "foo", 3);

   nread = read(sockfd, buf, sizeof(buf));
   printf("%s", buf);
   close(sockfd);
}


void UDPechoc(const char* host) {
   char* service = "echo";
   int nread;
   char buf[LINELEN+1] = {0};
   int sockfd = connectUDP(host, service);


   char req[LINELEN];
   char res[LINELEN];

    fgets(req, LINELEN, stdin);
    req[strcspn(req, "\n")] = 0; // remove trailing newline character
    write(sockfd, req, strlen(req));
    nread = read(sockfd, res, LINELEN);
    res[nread] = '\0'; // make sure string is null-terminated
    printf("%s \n", res);
    close(sockfd);
}

void TCPechoc(const char* host) {
    /* char* service = "echo"; */
    char* service = "4001";
    int n;
    int sockfd = connectTCP(host, service);
    char req[LINELEN];
    char res[LINELEN];
    while (1) {
        fgets(req, LINELEN, stdin);
        req[strcspn(req, "\n")] = 0; // remove trailing newline character
        write(sockfd, req, strlen(req));
        n = read(sockfd, res, LINELEN);
        res[n] = '\0'; // make sure string is null-terminated
        printf("%s \n", res);
    }
    close(sockfd);
}


void TCPtimec(const char* host) {
    /* char* service = "time"; */
    char* service = "4004";
    int nread;
    time_t now; // holds up to 8 bytes
    int sockfd = connectTCP(host, service);
    write(sockfd, "foo", 3);
    // Expecting to only receive 4 bytes integer representation of time from server
    /* nread = read(sockfd, &now, sizeof(now)); // doesn't have to be a char*, any void* pointer would work */
    printf("[tcp_time] Sending TIME request to the server\n");
    if (send(sockfd, MSG, strlen(MSG), 0) < 0) {
        errexit("[error] Time message send failed: %s\n",strerror(errno));
    }
    while ( (nread = recv(sockfd, (char *)&now, sizeof(now), 0)) > 0 ) {}
    printf("[tcp_time] Receive TIME response from the server\n");
    now = ntohl((u_long)now); /* put in host byte order */
    now -= UNIXEPOCH; /* convert UCT to UNIX epoch */
    printf("%s\n", ctime(&now));
    close(sockfd);
}

void TCPdaytimec(const char* host) {
   /* char* service = "daytime"; */
   char* service = "4003";
   int nread;
   char buf[LINELEN+1] = {0};
   int sockfd = connectTCP(host, service);

   write(sockfd, "foo", 3);
   nread = read(sockfd, buf, sizeof(buf));
   printf("%s\n", buf);
   close(sockfd);
}

void TCPchargenc(const char* host) {
    /* char* service = "chargen"; */
    char* service = "4002";
    int nread;
    char buf[LINELEN+1] = {0};
    int sockfd = connectTCP(host, service);
    while ((nread = read(sockfd, buf, sizeof(buf))) > 0 ) {
        buf[nread] = '\0'; // make sure it is null terminated
        printf("%s", buf);
    }
    close(sockfd);
}

int handleConnection(const char* host, const char* transport, const char* service) {
    if (strcmp(transport, "tcp") == 0) {
        if (strcmp(service, "time") == 0) {
            TCPtimec(host);
        } else if (strcmp(service, "daytime") == 0) {
            TCPdaytimec(host);
        } else if (strcmp(service, "echo") == 0) {
            TCPechoc(host);
        } else if (strcmp(service, "chargen") == 0) {
            TCPchargenc(host);
        } else {
            errexit("Service %s not supported", service);
        }
    } else if (strcmp(transport, "udp") == 0) {
        if (strcmp(service, "time") == 0) {
            UDPtimec(host);
        } else if (strcmp(service, "daytime") == 0) {
            UDPdaytimec(host);
        } else if (strcmp(service, "echo") == 0) {
            UDPechoc(host);
        } else if (strcmp(service, "chargen") == 0) {
            UDPchargenc(host);
        } else {
            errexit("Service %s not supported", service);
        }
    }
    else /* default: */ {
        errexit("Transport %s not supported", transport);
    }
}

int main(int argc, char* argv[]) {
    char* service;
    char* transport;
    char* host = "localhost";
    switch(argc) {
        case 3:
            transport = argv[1];
            service = argv[2];
            break;
        default:
            errexit("Not enough argument");
    }
    handleConnection(host, transport, service);
    return 0;
}
