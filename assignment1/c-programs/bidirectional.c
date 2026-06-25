#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main() {
    int pipe1[2];
    int pipe2[2];
    int pid;
    char parent_message[] = "Hello from parent!";
    char child_message[] = "Hello from child!";
    char buffer[100];
    if (pipe(pipe1) == -1) {
        printf("Error creating pipe1\n");
        return 1;
    }
    if (pipe(pipe2) == -1) {
        printf("Error creating pipe2\n");
        return 1;
    }
    pid = fork();
    if (pid == -1) {
        printf("Error creating child process\n");
        return 1;
    }
    else if (pid == 0) {
        close(pipe1[1]);
        close(pipe2[0]);
        read(pipe1[0], buffer, 100);
        printf("Child received: %s\n", buffer);
        write(pipe2[1], child_message, 18);
        close(pipe1[0]);
        close(pipe2[1]);
    }
    else {
        close(pipe1[0]);
        close(pipe2[1]);
        write(pipe1[1], parent_message, 19);
        read(pipe2[0], buffer, 100);
        printf("Parent received: %s\n", buffer);
        close(pipe1[1]);
        close(pipe2[0]);
        wait(NULL);
    }
    return 0;
}
