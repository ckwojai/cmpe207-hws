#include "active.h"

int main() {
   const char* host = "ut1-wwv.nist.gov";
   const char* service = "daytime";

   int sockfd = connectTCP(host, service);

   return 0;
}
