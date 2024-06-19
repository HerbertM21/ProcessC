#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

void sigpipe_handler(int sig) {
    printf("Caught SIGPIPE signal\n");
    exit(1);
}

int main(int argc, char** argv) {
    int fd[2];
    int pid, status;

    // Crear tubería
    pipe(fd);

    // Fork
    if ((pid = fork()) == 0) {
        // Proceso hijo
        printf("Child process: Created\n");

        // Cerrar extremo de lectura (no usado)
        close(fd[0]);

        // Escribir en la tubería
        int number = 900;
        write(fd[1], &number, sizeof(int));
        printf("Child process: Number written\n");

        // Cerrar extremo de escritura
        close(fd[1]);
        exit(0);
    } else {
        // Proceso padre
        printf("Father process\n");

        // Cerrar extremo de escritura (no usado)
        close(fd[1]);

        // Instalar el manejador de señal SIGPIPE
        signal(SIGPIPE, sigpipe_handler);

        // Leer de la tubería (el otro extremo se cerró)
        int number;
        ssize_t bytesRead = read(fd[0], &number, sizeof(int));

        if (bytesRead == 0) {
            printf("Father process: EOF reached (other end closed)\n");
        } else {
            printf("Father process: Number read %d\n", number);
        }

        // Cerrar extremo de lectura
        close(fd[0]);

        // Esperar al hijo
        wait(&status);
    }

    return 0;
}