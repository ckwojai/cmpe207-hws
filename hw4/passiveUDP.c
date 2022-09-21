#include "passiveSocket.h"

int passiveUDP(const char* service) {
    return passivesock(service, "udp", 0);
}
