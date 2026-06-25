#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>

volatile sig_atomic_t shutdown_flag = 0;

long bytes_sent = 0;

void handle_sigint(int sig) {
    shutdown_flag = 1;
}

void handle_sigusr1(int sig) {
    fprintf(stderr,
            "\nProducer Statistics\nBytes sent: %ld\n",
            bytes_sent);
}

int main(int argc, char *argv[]) {
    int opt;
    char *filename = NULL;
    int buffer_size = 4096;

    struct sigaction sa_int;
    struct sigaction sa_usr1;

    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);

    sa_usr1.sa_handler = handle_sigusr1;
    sigemptyset(&sa_usr1.sa_mask);
    sa_usr1.sa_flags = 0;
    sigaction(SIGUSR1, &sa_usr1, NULL);

    while ((opt = getopt(argc, argv, "f:b:")) != -1) {
        switch (opt) {
            case 'f':
                filename = optarg;
                break;

            case 'b':
                buffer_size = atoi(optarg);
                break;

            default:
                fprintf(stderr,
                        "Usage: %s [-f file] [-b size]\n",
                        argv[0]);
                return 1;
        }
    }

    FILE *input;

    if (filename != NULL) {
        input = fopen(filename, "r");

        if (input == NULL) {
            fprintf(stderr,
                    "Could not open file %s\n",
                    filename);
            return 1;
        }
    }
    else {
        input = stdin;
    }

    char *buffer = malloc(buffer_size);

    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");

        if (filename != NULL) {
            fclose(input);
        }

        return 1;
    }

    size_t bytes_read;

    while (!shutdown_flag &&
           (bytes_read = fread(buffer,
                               1,
                               buffer_size,
                               input)) > 0) {

        fwrite(buffer, 1, bytes_read, stdout);

        bytes_sent += bytes_read;
    }

    fprintf(stderr,
            "\nProducer exiting\nBytes sent: %ld\n",
            bytes_sent);

    free(buffer);

    if (filename != NULL) {
        fclose(input);
    }

    return 0;
}
