#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

char* timestamp() {
    char* timestamp_buffer = malloc(100);
    struct timeval tv;
    gettimeofday(&tv, NULL);
    strftime(timestamp_buffer, 100, "%Y:%m:%d %H:%M:%S", localtime(&tv.tv_sec));
    sprintf(timestamp_buffer + strlen(timestamp_buffer), ":%ld", tv.tv_usec);
    return timestamp_buffer;
}

void create_child_processes(int num_processes) {
    if (num_processes == 0) {
        return;
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("Error al crear el proceso hijo");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        printf("[%s] Child process (PID=%d)\n", timestamp(), getpid());
        create_child_processes(num_processes - 1);
        exit(EXIT_SUCCESS);
    } else {
        printf("[%s] Father process (PID=%d)\n", timestamp(), getpid());
        wait(NULL);
    }
}

int main(int argc, char** argv) {
    create_child_processes(50);
    return EXIT_SUCCESS;
}