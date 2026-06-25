#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
int main(int argc, char *argv[]) {
    int opt;
    char *filename = NULL;
    int buffer_size = 4096;
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
                        "Usage: %s [-f filename] [-b size]\n",
                        argv[0]);
                return 1;
        }
    }
    FILE *input;
    if (filename != NULL) {
        input = fopen(filename, "r");
        if (input == NULL) {
            fprintf(stderr,
                    "Error: Could not open file %s\n",
                    filename);
            return 1;
        }
    } else {
        input = stdin;
    }
    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), input) != NULL) {
        printf("%s", buffer);
    }
    if (filename != NULL) {
        fclose(input);
    }
    return 0;
}
