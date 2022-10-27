#include "passiveSocket.h"
#include "errexit.h"

#include <sys/socket.h> //socket()
#include <sys/types.h> // addrino
#include <netinet/in.h> // sockaddr_in struct
#include <netdb.h> // getprotobyname(), getservbyname()
#include <string.h> // strcmp()
#include <errno.h> // strerror()
#include <unistd.h> // close()

#define ADDR_FAM AF_INET

int passivesock(const char* service, const char* transport, int qlen) {
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
    hints.ai_flags = AI_PASSIVE; // for binding socket to accept incoming connection
    // 2nd arg will match service -> port number, hints's socktype will match name -> protocol
    int status = getaddrinfo(NULL, service, &hints, &addrinfo); // nodename is Null for an AI_PASSIVE
    if (status != 0) {
        errexit("can't getaddrinfo: %s \n", gai_strerror(status));
    }
    // Create socket
    sockfd = socket(ADDR_FAM, type, addrinfo->ai_protocol);
    // Bind socket
    int flag = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
    if (bind(sockfd, addrinfo->ai_addr, addrinfo->ai_addrlen) < 0) {
        close(sockfd);
        errexit("can't bind to %s port: %s\n", service, strerror(errno)); // strerror converts errno to readable string
    }
    if (type == SOCK_STREAM && listen(sockfd, qlen) < 0) { // TODO: understand this statement, qlen is only used here
        close(sockfd);
        errexit("can't listen on %s port: %s\n", service, strerror(errno));
    }
    // Free addrinfo
    freeaddrinfo(addrinfo);
    return sockfd; // consumer of this function need to close this socket using: close(sockfd)
}
