#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
int main() {
int pipefd[2];
pid_t pid;
char buffer[100];
char *message = "Hello from parent!";
if (pid == 0) {
} else {
}
return 0;
}
