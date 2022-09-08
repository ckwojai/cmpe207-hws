#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

bool readReverse() {
  char buf[100];
  int f_read = open("start.txt", O_RDONLY);
  // int f_write = open("end.txt", O_WRONLY);
  //
  //
  int n = lseek(f_read, -1, SEEK_END);
  char line[100];
  int line_len = 0;
  int count = 0;
  char c;
  while (n>=0)  {
       read(f_read, &c, 1);
       if (c == '\n') {
         printf("FOUND NEW LINE, Count=%d", count);
         read(f_read, line, count);
         printf("LINE READ: %s", line);
         count = 0;
       }
       n = lseek(f_read, -2, SEEK_CUR);
       // write(fdout, &c, 1);
  }
  // lseek(f_read, -9, SEEK_END);
  // read(f_read, buf, 7);
  // printf("%s", buf);
  return true;
}
int main() {
  readReverse();
  return 0;
}

