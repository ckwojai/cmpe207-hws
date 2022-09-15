#include "active.h"
#include <unistd.h> // close()

int main() {
   const char* host = "ut1-wwv.nist.gov";
   const char* service = "daytime";

   int sockfd = connectTCP(host, service);

   close(sockfd);
   return 0;
}
