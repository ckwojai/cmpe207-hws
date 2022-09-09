#include <fcntl.h> // O_RDONLY...
#include <stdio.h> // printf()
#include <stdlib.h> // calloc(), fopen()...
#include <unistd.h> // for types
#include <string.h> // strlen()
#include <time.h> // clock()

#define MAX_LINE_LEN 2000

bool reverse_file_by_c(char* input_file_name, char* output_file_name) {
    FILE* f_read = fopen(input_file_name, "r");
    FILE* f_write = fopen(output_file_name, "a+");

    // calloc() allocates the memory and also initializes every byte in the allocated memory to 0.
    char *line = (char *) calloc(MAX_LINE_LEN, sizeof(char));
    int line_len = 0;
    char c;
    int n = fseek(f_read, -1, SEEK_END); // -1 to read the last character
    fread(&c, 1, 1, f_read);
    if (c == '\n') { // if file ends with a new line character
        n = fseek(f_read, -2, SEEK_END);
        fwrite("\n", 1, 1, f_write);
    }
    while (n>=0)  {
        // read char one by one and check if it is a new line
        fread(&c, 1, 1, f_read);
        if (c == '\n') {
            // if we find a new line char, read entire line and write to file
            fread(line, line_len, 1, f_read);
            fwrite(line, strlen(line), 1, f_write);
            fwrite("\n", 1, 1, f_write);
            // -2 to move cursor back to one char before the last found new line
            n = fseek(f_read, (-1*line_len)-2, SEEK_CUR);
            // reset line so that it can be used again
            // use strlen instead of sizeof, sizeof(line) will alwasy return the pointer size (8)
            memset(line, '\0', strlen(line));
            line_len = 0;
            continue;
        }
        line_len++;
        n = fseek(f_read, -2, SEEK_CUR);
    }
    // Read the first line after the while loop
    n = fseek(f_read, 0, SEEK_SET);
    fread(line, line_len, 1, f_read);
    fwrite(line, strlen(line), 1, f_write);
    fwrite("\n", 1, 1, f_write);

    free(line); // free calloc-ed memory
    return true;
}

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
    char in_name[] = "lpsum100kB.txt";
    char out_name_c[] = "lpsum100kB_rev_by_c";
    char out_name_unix[] = "lpsum100kB_rev_by_unix";

    clock_t tic = clock();
    reverse_file_by_unix(in_name, out_name_unix);
    clock_t toc = clock();
    printf("Using Unix: Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);

    tic = clock();
    reverse_file_by_c(in_name, out_name_c);
    toc = clock();
    printf("Using C: Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
    return 0;
}

