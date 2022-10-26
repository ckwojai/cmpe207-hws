#include "activeSocket.h"

int connectUDP(const char* host, const char* service) {
    return connectsock(host, service, "udp");
}
