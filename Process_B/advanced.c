#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void sigint_handler(int sig) {
    printf("Señal SIGINT recibida\n");
    exit(0);
}

int main() {
    pid_t pid;
    int status;

    // Registrar el manejador de señal SIGINT
    signal(SIGINT, sigint_handler);

    // Crear un nuevo proceso hijo
    pid = fork(); // En el proceso hijo devuelve 0 como valor de retorno, en el padre devuelve el PID del hijo

    if (pid == -1) {
        perror("Error al crear el proceso hijo");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Proceso hijo
        printf("Soy el proceso hijo con PID: %d\n", getpid());
        printf("Proceso hijo durmiendo por 5 segundos\n");
        sleep(5);
        printf("Proceso hijo terminado\n");
        exit(EXIT_SUCCESS);
    } else {
        // Proceso padre
        printf("Soy el proceso padre con PID: %d\n", getpid());
        printf("He creado el proceso hijo con PID: %d\n", pid);

        // Esperar a que el proceso hijo termine
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            printf("El proceso hijo terminó con código de salida: %d\n", exit_status);
        } else {
            printf("El proceso hijo terminó de forma anormal\n");
        }
    }

    return 0;
}