#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <signal.h>

volatile sig_atomic_t shutdown_flag = 0;

int line_count = 0;
int char_count = 0;

void handle_sigint(int sig) {
    shutdown_flag = 1;
}

void handle_sigusr1(int sig) {
    fprintf(stderr,
            "\nCurrent Statistics\n");
    fprintf(stderr,
            "Lines: %d\n",
            line_count);
    fprintf(stderr,
            "Characters: %d\n",
            char_count);
}

int main(int argc, char *argv[]) {
    int opt;
    int max_lines = -1;
    int verbose = 0;

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

    while ((opt = getopt(argc, argv, "n:v")) != -1) {
        switch (opt) {
            case 'n':
                max_lines = atoi(optarg);
                break;

            case 'v':
                verbose = 1;
                break;

            default:
                fprintf(stderr,
                        "Usage: %s [-n max_lines] [-v]\n",
                        argv[0]);
                return 1;
        }
    }

    char line[4096];
    int i;

    while (!shutdown_flag &&
           fgets(line, sizeof(line), stdin) != NULL) {

        if (max_lines != -1 &&
            line_count >= max_lines) {
            break;
        }

        line_count++;

        i = 0;

        while (line[i] != '\0') {
            char_count++;
            i++;
        }

        if (verbose) {
            printf("%s", line);
        }
    }

    fprintf(stderr,
            "\nFinal Statistics\n");
    fprintf(stderr,
            "Lines processed: %d\n",
            line_count);
    fprintf(stderr,
            "Characters processed: %d\n",
            char_count);

    return 0;
}
