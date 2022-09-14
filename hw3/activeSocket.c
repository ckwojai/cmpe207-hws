#include "activeSocket.h"
#include <netinet/in.h> // sockaddr_in struct
#include <netdb.h> // getprotobyname(), getservbyname()
#include <string.h> // strcmp()
#include <arpa/inet.h> // inet_pton()

#define ADDR_FAM AF_INET

int connectsock(const char* host, const char* service, const char* transport) {
    int sockfd, type;

    struct protoent* ppe; // pointer to protocol info entry
    // Get protocol number from protocol name
    if ((ppe = getprotobyname(transport)) == 0) {
        // error handling
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
        // error handling
    }
    // convert hostname to hostaddr and copy to sin
    inet_pton(ADDR_FAM, host, &sin.sin_addr); // example of host: "google.com"


    // Create socket
    sockfd = socket(ADDR_FAM, type, ppe->p_proto);
    // Create connection
    if (connect(sockfd, (struct sockaddr*) &sin, sizeof(sin)) < 0)
        // error handling

    return sockfd;
}
