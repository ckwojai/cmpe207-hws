#include "activeSocket.h"
#include "errexit.h"

#include <sys/socket.h> //socket()
#include <sys/types.h> // addrino
#include <netinet/in.h> // sockaddr_in struct
#include <netdb.h> // getprotobyname(), getservbyname()
#include <string.h> // strcmp()
#include <arpa/inet.h> // inet_pton()
#include <errno.h> // strerror()
#include <unistd.h> // close()

#define ADDR_FAM AF_INET

int connectsock(const char* host, const char* service, const char* transport) {
    int sockfd, type;

    if (strcmp(transport, "udp") == 0)
        type = SOCK_DGRAM;
    else
        type = SOCK_STREAM;

    struct addrinfo* addrinfo;
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_socktype = type;
    hints.ai_family = PF_UNSPEC;
    int status = getaddrinfo(host, service, &hints, &addrinfo);
    if (status != 0) {
        errexit("can't getaddrinfo: %s \n", gai_strerror(status));
    }
    // Create socket
    sockfd = socket(ADDR_FAM, type, addrinfo->ai_protocol);
    // Create connection
    if (connect(sockfd, addrinfo->ai_addr, addrinfo->ai_addrlen) < 0) {
        close(sockfd);
        errexit("can't connect to %s of %s: %s\n", service, host, strerror(errno)); // strerror converts errno to readable string
    }
    // Free addrinfo
    freeaddrinfo(addrinfo);
    return sockfd; // consumer of this function need to close this socket using: close(sockfd)
}
