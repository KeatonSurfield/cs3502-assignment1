#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main() {
    int pipefd[2];
    int pid;
    char buffer[100];
    char *message = "Hello from parent!";
    if (pipe(pipefd) == -1) {
        printf("Error creating pipe\n");
        return 1;
    }
    pid = fork();
    if (pid == -1) {
        printf("Error creating child process\n");
        return 1;
    }
    if (pid == 0) {
        close(pipefd[1]);
        read(pipefd[0], buffer, 100);
        printf("Child received: %s\n", buffer);
        close(pipefd[0]);
    }
    else {
        close(pipefd[0]);
        write(pipefd[1], message, 19);
        close(pipefd[1]);
        wait(NULL);
    }
    return 0;
}
