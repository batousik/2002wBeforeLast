#include "sp1.h"


int main(int argc, char **argv) {
    char phrase[] = "Alala Pasha codoncodonkina";
    return chinese_whisper(phrase, 10);
}

int chinese_whisper(char phrase[], int repeat_times){
    pid_t pid;
    int whisper_pipe[2];

    /* Create the pipe. */
    if (pipe (whisper_pipe)) {
        fprintf (stderr, "Pipe failed.\n");
        return EXIT_FAILURE;
    }

    /* Create the child process. */
    pid = fork ();
    if (pid == (pid_t) 0) {
        /* This is the child process.
           Close other end first. */
        close (whisper_pipe[1]);
        listen_to_whisper(whisper_pipe[0]);
        return EXIT_SUCCESS;
    } else if (pid > (pid_t) 0) {
        /* This is the parent process.
           Close other end first. */
        close (whisper_pipe[0]);
        tell_a_whisper(whisper_pipe[1], "asd");
        return EXIT_SUCCESS;
    } else {
        /* The fork failed. */
        fprintf (stderr, "Fork failed.\n");
        return EXIT_FAILURE;
    }
}

/* Read and return text from pipe. */
char* listen_to_whisper(int file) {
    char *whisper = "";
    char *buffer;
    size_t len;
    FILE *stream;
    int c;
    stream = fdopen (file, "r");
    while ((c = fgetc (stream)) != EOF){
        printf("suka: %c", c);
        // get length of currently read string
        len = strlen(whisper);
        // allocate space for that string + 1
        buffer = malloc(len + 1);
        // copy existing whisper to a buffer
        strcpy(buffer, whisper);
        // add new read character
        buffer[len] = c;
        // reset whisper to the buffer
        free(whisper);
        whisper = malloc(strlen(buffer));
        strcpy(whisper, buffer);
        // free buffer
        free(buffer);
    }
    // add string end symbol
    // get length of currently read string
    len = strlen(whisper);
    // allocate space for that string + 1
    buffer = malloc(len + 1);
    // copy existing whisper to a buffer
    strcpy(buffer, whisper);
    // add new read character
    buffer[len] = '\0';
    // reset whisper to the buffer
    free(whisper);
    whisper = malloc(strlen(buffer));
    strcpy(whisper, buffer);
    // free buffer
    free(buffer);
    fclose (stream);
    printf("Read a whisper from pipe: %s", whisper);
    return whisper;
}

/* Write whisper to the pipe. */
void tell_a_whisper(int file, char whisper[]) {
    FILE *stream;
    stream = fdopen (file, "w");
    printf("Got a whisper to write to pipe: %s", whisper);
    fprintf (stream, whisper);
    fclose (stream);
}
