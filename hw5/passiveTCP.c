#include "passiveSocket.h"

int passiveTCP(const char* service, int qlen) {
    return passivesock(service, "tcp", qlen);
}
