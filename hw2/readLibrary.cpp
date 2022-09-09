#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>

bool readReverse() {
  char buf[100];
  int f_read = open("start.txt", O_RDONLY);
  int f_write = open("end.txt", O_WRONLY | O_CREAT, 0666);

  int n = lseek(f_read, -1, SEEK_END);
  char *line = (char *) calloc(100, sizeof(char));
  int line_len = 0;
  char c;
  read(f_read, &c, 1);
  if (c == '\n') {
    n = lseek(f_read, -2, SEEK_END);
    write(f_write, "\n", 1);
  }
  while (n>=0)  {
       read(f_read, &c, 1);
       if (c == '\n') {
         printf("FOUND NEW LINE, Count=%d \n", line_len);
         read(f_read, line, line_len);
	 n = lseek(f_read, (-1*line_len)-2, SEEK_CUR);

	 write(f_write, line, strlen(line));
	 write(f_write, "\n", 1);
	 memset(&line[0], '\0', strlen(line));
         line_len = 0;
	 continue;
       }
       line_len++;
       n = lseek(f_read, -2, SEEK_CUR);
  }
  n = lseek(f_read, 0, SEEK_SET);
  read(f_read, line, line_len);
  write(f_write, line, strlen(line));
  write(f_write, "\n", 1);
  free(line);
  return true;
}
int main() {
  readReverse();
  return 0;
}

