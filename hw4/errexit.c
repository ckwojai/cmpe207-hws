#include "errexit.h"
#include <stdlib.h> // exit()
#include <stdio.h> //
#include <stdarg.h> // types like va_list, va_start...

void errexit(const char* format, ...) {
    va_list args;
    va_start(args, format);
    // sends formatted output to a stream using an argument list passed to it.
    vfprintf(stderr, format, args);
    va_end(args);
    // WSACleanup(); <-- not needed for Linux system
    // close(int sockfd) <-- not needed because errexit is called before creation of socket
    exit(1);
}
