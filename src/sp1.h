#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int chinese_whisper(char phrase[], int repeat_times);
char* listen_to_whisper(int file);
void tell_a_whisper(int file, char whisper[]);
