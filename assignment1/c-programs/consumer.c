#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
int main(int argc, char *argv[]) {
    int opt;
    int max_lines = -1;
    int verbose = 0;
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
    int line_count = 0;
    int char_count = 0;
    int i;
    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (max_lines != -1 && line_count >= max_lines) {
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
    fprintf(stderr, "Lines processed: %d\n", line_count);
    fprintf(stderr, "Characters processed: %d\n", char_count);
    return 0;
}
