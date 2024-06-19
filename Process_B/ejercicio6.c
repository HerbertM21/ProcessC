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

int main(int argc, char** argv) {
    int status;
    pid_t pid;
    pid_t pids_hijos[50];

    // Imprimir el timestamp del padre una sola vez
    printf("[%s] Proceso padre (PID=%d)\n", timestamp(), getpid());

    // Se crean 50 procesos hijos dentro de un bucle
    for (int i = 0; i < 50; i++) {
        pid = fork();
        if (pid == -1) {
            printf("Error al crear el proceso hijo");
            exit(1);
        } else if (pid == 0) {
            // Proceso hijo
            printf("[%s] Proceso hijo (PID=%d)\n", timestamp(), getpid());
            exit(0);
        } else {
            // Se guardan los PID de los hijos en el proceso padre
            // fork() devuelve 0 en el hijo y el PID del hijo en el padre
            pids_hijos[i] = pid;
        }
    }

    // Esperamos que finalicen todos los hijos para finalizar el padre
    for (int i = 0; i < 50; i++) {
        waitpid(pids_hijos[i], &status, 0);
    }

    printf("[%s] Proceso padre finalizado\n", timestamp());

    return 0;
}