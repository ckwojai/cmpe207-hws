#include "activeSocket.h"

int connectTCP(const char* host, const char* service) {
    return connectsock(host, service, "tcp");
}
