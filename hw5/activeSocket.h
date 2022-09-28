#ifndef ACTIVESOCKET_H_
#define ACTIVESOCKET_H_

#include <sys/socket.h>
#include <sys/types.h>


int connectsock(const char* host, const char* service, const char* transport);

#endif // ACTIVESOCKET_H_
