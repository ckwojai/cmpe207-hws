#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE_LEN 100

bool reverse_file_by_unix(char* input_file_name, char* output_file_name) {
    int f_read = open(input_file_name, O_RDONLY);
    int f_write = open(output_file_name, O_WRONLY | O_CREAT, 0666); // 0666 for permission

    // calloc() allocates the memory and also initializes every byte in the allocated memory to 0.
    char *line = (char *) calloc(MAX_LINE_LEN, sizeof(char));
    int line_len = 0;
    char c;
    int n = lseek(f_read, -1, SEEK_END); // -1 to read the last character
    read(f_read, &c, 1);
    if (c == '\n') { // if file ends with a new line character
        n = lseek(f_read, -2, SEEK_END);
        write(f_write, "\n", 1);
    }
    while (n>=0)  {
        // read char one by one and check if it is a new line
        read(f_read, &c, 1);
        if (c == '\n') {
            // if we find a new line char, read entire line and write to file
            read(f_read, line, line_len);
            write(f_write, line, strlen(line));
            write(f_write, "\n", 1); // manually add new line char
            // -2 to move cursor back to one char before the last found new line
            n = lseek(f_read, (-1*line_len)-2, SEEK_CUR);
            // reset line so that it can be used again
            // use strlen instead of sizeof, sizeof(line) will alwasy return the pointer size (8)
            memset(line, '\0', strlen(line));
            line_len = 0;
            continue;
        }
        line_len++;
        n = lseek(f_read, -2, SEEK_CUR);
    }
    // Read the first line after the while loop
    n = lseek(f_read, 0, SEEK_SET);
    read(f_read, line, line_len);
    write(f_write, line, strlen(line));
    write(f_write, "\n", 1);

    free(line); // free calloc-ed memory
    return true;
}
int main() {
    char in_name[] = "lpsum100.txt";
    char out_name_c[] = "lpsum100_rev_by_c";
    char out_name_unix[] = "lpsum100_rev_by_unix";
    reverse_file_by_unix(in_name, out_name_unix);
    return 0;
}

