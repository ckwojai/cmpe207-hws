#include "activeSocket.h"
#include "errexit.h"

#include <netinet/in.h> // sockaddr_in struct
#include <netdb.h> // getprotobyname(), getservbyname()
#include <string.h> // strcmp()
#include <arpa/inet.h> // inet_pton()
#include <errno.h> // strerror()

#define ADDR_FAM AF_INET

int connectsock(const char* host, const char* service, const char* transport) {
    int sockfd, type;

    struct protoent* ppe; // pointer to protocol info entry
    // Get protocol number from protocol name
    if ((ppe = getprotobyname(transport)) == 0) {
        errexit("can't get \"%s\" protocol entry\n", transport);
    }

    if (strcmp(transport, "udp") == 0)
        type = SOCK_DGRAM;
    else
        type = SOCK_STREAM;

    // for connection
    struct sockaddr_in sin;
    // pointer to service info entry
    struct servent* pse;
    if ((pse = getservbyname(service, transport))) { // example of service: "daytime"
        sin.sin_port = pse->s_port; // service port number, different than protocol number
    } else {
        errexit("can't get \"%s\" service entry\n", service);
    }

    // convert hostname to hostaddr and copy to sin
    if (inet_pton(AF_INET, host, &sin.sin_addr) <= 0) {
        errexit("can't get \"%s\" host entry. %s\n", host, strerror(errno));
    } // example of host: "google.com"


    // Create socket
    sockfd = socket(ADDR_FAM, type, ppe->p_proto);
    // Create connection
    if (connect(sockfd, (struct sockaddr*) &sin, sizeof(sin)) < 0)
        errexit("can't connect to &s.%s: %s\n", host, service, strerror(errno)); // strerror converts errno to readable string

    return sockfd; // consumer of this function need to close this socket using: close(sockfd)
}
