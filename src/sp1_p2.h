#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>

int chinese_whisper(int repeat_times, char *whisper);
char* listen_to_whisper(int file);
void tell_a_whisper(int file, char *whisper);
void swap_chars_in_string(int *a, int *b, char *string);