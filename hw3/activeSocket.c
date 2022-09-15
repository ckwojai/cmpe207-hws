#include "activeSocket.h"
#include "errexit.h"

#include <sys/socket.h> //socket()
#include <sys/types.h> // addrino
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
    /* if ((ppe = getprotobyname(transport)) == 0) { */
    /*     errexit("can't get \"%s\" protocol entry\n", transport); */
    /* } */

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
    /* - service determines the port */
    /* - hints.socktype determines the protocol */
    /* - host, together with the above get combined to sockaddr in .ai_addr that can be used to create socket directly */

    /* // for connection */
    /* struct sockaddr_in sin; */
    /* // pointer to service info entry */
    /* struct servent* pse; */
    /* if ((pse = getservbyname(service, transport))) { // example of service: "daytime" */
    /*     sin.sin_port = pse->s_port; // service port number, different than protocol number */
    /* } else { */
    /*     errexit("can't get \"%s\" service entry\n", service); */
    /* } */

    /* // convert hostname to hostaddr and copy to sin */
    /* if (inet_pton(AF_INET, host, &sin.sin_addr) <= 0) { */
    /*     errexit("can't get \"%s\" host entry. %s\n", host, strerror(errno)); */
    /* } // example of host: "google.com" */


    // Create socket
    sockfd = socket(ADDR_FAM, type, addrinfo->ai_protocol);
    // Create connection
    if (connect(sockfd, addrinfo->ai_addr, addrinfo->ai_addrlen) < 0)
        errexit("can't connect to &s.%s: %s\n", host, service, strerror(errno)); // strerror converts errno to readable string

    // Free addrinfo
    freeaddrinfo(addrinfo);
    freeaddrinfo(&hints);
    return sockfd; // consumer of this function need to close this socket using: close(sockfd)
}
