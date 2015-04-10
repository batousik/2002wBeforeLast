#include "sp1_p2.h"

const char * const not_string = "pcO65gAxOBzrKz3rThHUnnbXCuJwMsRP50nPbo3l8NkhtxsDMBGmQbmEEviFndMaUkwRimgHBK1bwT47BgGswlx5fVUtFcCYEHKmRIbr4mSbIDiSIAYuQ0POLgSbBXKv";
char ***filters;
int program_pid;
int num_filters;
int main_cnt;

int main(int argc, char *argv[]) {
    printf("Program started, pid: %i, ppid: %i\n", getpid(), getppid());
    program_pid = getpid();
    main_cnt = 0;
    // argc 0 is the name of executable
    // 5 filters => 10 arguments (0 - 10 is eleven)
    // so taking argc itself for calculation ignores value at 0
    if (!(argc % 2)) {
        printf("INVALID FILTER PARAMS\n");
        printf("\tEach filter has to have name andits value\n");
        printf("\tAll arguments has to be only separated by spaces\n");
        return EXIT_FAILURE;
    }

    num_filters = argc/2;

    // create filters array
    // first argv is actually name of executable
    // each filter is a pointer to array[2] of char*
    filters = malloc(num_filters * sizeof(char**));

    int loc = 0;

    for (int i = 0; i < argc/2; i++) {
        // Create program workflow according to filters
        int j = i + 1;
        // allocating memory for array of filters,
        // each filter has a pointer to filter name and filter value
        filters[i] = malloc(2 * sizeof(char*));
        char **filter_set;
        filter_set = filters[i];
        for (int k = 0; k < 2; ++k) {
            loc = j * 2 - 1 + k;
            int f_length = strlen(argv[loc]);
            // allocating memory for actual name and filter value (+1 for \0)
            filter_set[k] = malloc(sizeof(char) * (f_length + 1));
            char *data;
            data = filter_set[k];
            // setting all values in the string holder to be end of string
            memset(data, '\0', sizeof(char) * (f_length + 1));
            // copying actual values
            strcpy(data, argv[loc]);
        }
    }
    // debug
    for (int l = 0; l < num_filters; ++l) {
        printf("%i: %s = %s\n", l, filters[l][0], filters[l][1]);
    }

    // read stdin loop
    pid_t pid;
    int my_pid;
    int parent_pid = getpid();
    int phrase_pipe[2];

    /* Create the pipe. */
    if (pipe (phrase_pipe)) {
        fprintf (stderr, "Pipe failed.\n");
        return EXIT_FAILURE;
    }
    /* Create the child process. */
    pid = fork ();
    if (pid == (pid_t) 0) {
        // Child
        my_pid = getpid();
        // printf("New process %i, parent %i\n", my_pid, parent_pid);
        // close unused write end
        // close (phrase_pipe[1]);
        // initiate filtering
        do_filtering(0, phrase_pipe[0]);
        exit(EXIT_SUCCESS);
    } else if (pid > (pid_t) 0) {
        // Parent
        // close unused read end
        close (phrase_pipe[0]);
        char *filename = "test.txt";
        FILE *stream;
        stream = fopen (filename, "r");
        char *line = get_next_file_line(stream);
        // reading file and writing it to the pipe
        while (line != EOF) {
            printf("%s\n", line);
            write_line_to_pipe(phrase_pipe[1], line);
            write_line_to_pipe(phrase_pipe[1], "\n");
            line = get_next_file_line(stream);
        }
        fclose (stream);
        // wait for all children
        wait(NULL);
        // close write end
        close(phrase_pipe[1]);
        printf("DONE");
        exit(EXIT_SUCCESS);
    } else {
        /* The fork failed. */
        fprintf (stderr, "Fork failed.\n");
        return EXIT_FAILURE;
    }
}

int do_filtering(int cur_filter, int fd_read){
    printf("[DO FILTERING]\tPid: %i, PPid: %i, cur_filter: %i\n", getpid(), getppid(), cur_filter);
    pid_t pid;
    int my_pid;
    int parent_pid = getpid();
    int phrase_pipe[2];

    /* Create the pipe. */
    if (pipe (phrase_pipe)) {
        printf("VERYBAD");
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
       // close (phrase_pipe[1]);
        // read from pipe
        // char *recieved_phrase = get_next_line(phrase_pipe[0]);
        // printf("pid:%i received string[%s]\n", my_pid, recieved_phrase);
        // close read end
        // close(phrase_pipe[0]);
//        if(recieved_phrase == NULL){
//            printf("NULL\n");
//        }
        if (cur_filter < (num_filters-1)){
            return do_filtering(++cur_filter, phrase_pipe[0]);
        }
        exit(EXIT_SUCCESS);
    } else if (pid > (pid_t) 0) {
        // Parent
        printf("dead");
        close (phrase_pipe[0]);
        // keep reading lines from pipe
        char *line = get_next_line(fd_read);
        // reading file and writing it to the pipe
        while (line != EOF) {
            // if filter okay, pass on the line to children
            if (do_filter(cur_filter, line)) {
                // if current filter worked pipe a phrase
                write_line_to_pipe(phrase_pipe[1], line);
                write_line_to_pipe(phrase_pipe[1], '\n');
            }
            line = get_next_line(fd_read);
        }
        printf("dead");
        // write EOF to children
        write_line_to_pipe (phrase_pipe[1], EOF);
        // wait for all children
        wait(NULL);
        exit(EXIT_SUCCESS);
    } else {
        /* The fork failed. */
        fprintf (stderr, "Fork failed.\n");
        return EXIT_FAILURE;
    }
}

/* Read and return text from pipe. */
char *get_next_line(int file) {
    char *whisper = malloc(0);
    char *buffer;
    size_t len;
    FILE *stream;
    int c;
    stream = fdopen (file, "r");
    for (int j = 0; (c = fgetc (stream)) != '\n'; ++j) {
        if(c == EOF){
            fclose (stream);
            return c;
        }
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

/* Read and return text from input file line by line. */
char *get_next_file_line(FILE *stream) {
    char *a_line = malloc(0);
    char *buffer;
    size_t len;
    int c;
    for (int j = 0; (c = fgetc (stream)) != '\n'; ++j) {
        if(c == EOF){
            fclose (stream);
            return c;
        }
        // allocate space for current string + char
        buffer = malloc((j+2) * sizeof(char));
        // set all chars to \0
        memset(buffer, '\0', (j+2) * sizeof(char));
        // copy existing whisper to a buffer
        memcpy(buffer, a_line, j * sizeof(char));
        // add new read character to the end
        len = strlen(buffer);
        buffer[len] = c;
        // reset whisper to the buffer
        free(a_line);
        a_line = malloc((j+2) * sizeof(char));
        memcpy(a_line, buffer, (j+2) * sizeof(char));
        // free buffer
        free(buffer);
    }
    return a_line;
}


/* Write whisper to the pipe. */
void write_line_to_pipe(int file, char *whisper) {
    FILE *stream;
    stream = fdopen (file, "w");
    // printf("Got a whisper to write to pipe: %s\n", whisper);
    fprintf (stream, "%s", whisper);
    fclose (stream);
}


int do_filter(int cur_filter, char *phrase){
    printf("[DO FILTER]\tPid: %i, PPid: %i, cur_filter: %i, phrase: %s, filter: %s\n", getpid(), getppid(), cur_filter, phrase, filters[cur_filter][0]);

    if (strcmp(filters[cur_filter][0], "Contains") == 0){
        return 1;
    } else if (strcmp(filters[cur_filter][0], "Length") == 0){
        return 1;
    } else if (strcmp(filters[cur_filter][0], "AtLeast") == 0){
        return 1;
    } else if (strcmp(filters[cur_filter][0], "No") == 0){
        return 1;
    } else if (strcmp(filters[cur_filter][0], "Every") == 0){
        return 1;
    }
    // FILTER DOESNT EXIST
    return 0;
}