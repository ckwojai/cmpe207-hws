#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

bool readReverse() {
  char buf[100];
  int f_read = open("start.txt", O_RDONLY);
  int f_write = open("end.txt", O_WRONLY);
  lseek(f_read, -9, SEEK_END);
  read(f_read, buf, 7);
  printf("%s", buf);
  return true;
}
int main() {
  readReverse();
  return 0;
}

