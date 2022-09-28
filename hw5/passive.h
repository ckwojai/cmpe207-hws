#ifndef PASSIVE_H_
#define PASSIVE_H_

int passiveUDP(const char* service);
int passiveTCP(const char* service, int qlen);
void errexit(const char* format, ...);

#endif // PASSIVE_H_
