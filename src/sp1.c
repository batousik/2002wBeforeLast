#include "sp1.h"


int main(int argc, char *argv[]) {
    char *phrase = malloc(1024 * sizeof(char));
    memset(phrase, '\0', 1024);
    for (int i = 1; i < argc; ++i) {
        strcat(phrase, argv[i]);
        strcat(phrase, " ");
    }
    return do_filtering(190, phrase);
}

int do_filtering(int repeat_times, char *whisper){
    pid_t pid;
    int my_pid;
    int parent_pid = getpid();
    int whisper_pipe[2];

    /* Create the pipe. */
    if (pipe (whisper_pipe)) {
        fprintf (stderr, "Pipe failed.\n");
        return EXIT_FAILURE;
    }

    /* Create the child process. */
    pid = fork ();
    if (pid == (pid_t) 0) {
        // Child
        my_pid = getpid();
        printf("New process %i, parent %i\n", my_pid, parent_pid);
        // close unused write end
        close (whisper_pipe[1]);
        // read from pipe
        char* recieved_whisper = get_next_line(whisper_pipe[0]);
        printf("pid:%i received string[%s]\n", my_pid, recieved_whisper);
        // close read end
        close(whisper_pipe[0]);
        if (repeat_times > 0){
            return do_filtering(--repeat_times, recieved_whisper);
        }
        return EXIT_SUCCESS;
    } else if (pid > (pid_t) 0) {
        // Parent
        int *a, *b;
        a = malloc(sizeof(int));
        b = malloc(sizeof(int));
        // alter whisper
        swap_chars_in_string(a, b, whisper);
        printf("pid:%i Swapped indeces %i, %i\n", parent_pid, *a, *b);
        free(a);
        free(b);
        // close unused read end
        close (whisper_pipe[0]);
        // pipe a whisper
        write_line_to_pipe(whisper_pipe[1], whisper);
        // close write end
        close(whisper_pipe[1]);
        // wait for child
        wait(NULL);
        return EXIT_SUCCESS;
    } else {
        /* The fork failed. */
        fprintf (stderr, "Fork failed.\n");
        return EXIT_FAILURE;
    }
}

/* Read and return text from pipe. */
char*get_next_line(int file) {
    char *whisper = malloc(0);
    char *buffer;
    size_t len;
    FILE *stream;
    int c;
    stream = fdopen (file, "r");
    for (int j = 0; (c = fgetc (stream)) != EOF; ++j) {
        // allocate space for current string + char
        buffer = malloc((j+2) * sizeof(char));
        // set all chars to \0
        memset(buffer, '\0', (j+2) * sizeof(char));
        // copy existing whisper to a buffer
        memcpy(buffer, whisper, j * sizeof(char));
        // add new read character to the end
        len = strlen(buffer);
        buffer[len] = c;
        // reset whisper to the buffer
        free(whisper);
        whisper = malloc((j+2) * sizeof(char));
        memcpy(whisper, buffer, (j+2) * sizeof(char));
        // free buffer
        free(buffer);
    }
    fclose (stream);
    return whisper;
}


/* Write whisper to the pipe. */
void write_line_to_pipe(int file, char *whisper) {
    FILE *stream;
    stream = fdopen (file, "w");
    // printf("Got a whisper to write to pipe: %s\n", whisper);
    fprintf (stream, "%s", whisper);
    fclose (stream);
}

void swap_chars_in_string(int *a, int *b, char *string){
//    printf("swapping: %i\n", strlen(string));
    // initialise random generator with seed derived from current time
    srand(getpid());
    // get random indeces
    *a = rand() % strlen(string);
    *b = rand() % strlen(string);
    // makesure a and b are not the same index
    while(*a == *b)
        *b = rand() % strlen(string);
    // swap indeces
    char temp = string[*a];
    string[*a] = string[*b];
    string[*b] = temp;
}