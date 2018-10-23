#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <memory.h>
#include <zconf.h>

char **parsing(char *string, ssize_t size) {
    char **command = malloc(size * sizeof(char *));
    char *s;
    s = strtok(string, " \r\n\t");
    int i = 0;
    while (s != NULL) {
        command[i++] = s;
        s = strtok(NULL, " \r\n\t");
    }
    command[i] = NULL;
    return command;
}

void execute(char **arguments) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork error\n");
    }
    if (pid == 0) {
        if (execvp(arguments[0], arguments) == -1) {
            perror("wrong input\n");
        }
    } else if (pid > 0) {
        int status;
        pid_t wpid = waitpid(pid, &status, 0);
        printf("\nexit code: '%d'\n", status);
    }
}

int main(int argc, char **argv) {
    size_t buffer_size = 0;
    char *buffer;
    ssize_t size = 0;

    while (1) {
        printf(">>> ");
        size = getline(&buffer, &buffer_size, stdin);

        char **command = parsing(buffer, size);

        if (strcmp(command[0], "exitt") == 0) {
            break;
        }
        if (strcmp(command[0], "") != 0) {
            execute(command);
        }
    }
    return 0;
}