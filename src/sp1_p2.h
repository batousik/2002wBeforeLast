#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

int do_filtering(int repeat_times, int fd_read);
char*get_next_line(int file);
void write_line_to_pipe(int file, char *whisper);
void swap_chars_in_string(int *a, int *b, char *string);
int do_filter(int cur_filter, char *phrase);
char *get_next_file_line(FILE *stream);