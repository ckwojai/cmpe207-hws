#ifndef PASSIVESOCKET_H_
#define PASSIVESOCKET_H_

#include <sys/socket.h>
#include <sys/types.h>


int passivesock(const char* service, const char* transport, int qlen);

#endif // ACTIVESOCKET_H_
