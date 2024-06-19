#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

#define NUM_PROCESOS 4
#define RANGO 100

bool es_primo(int num) {
    if (num < 2) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}

void encontrar_primos(int inicio, int fin, int pipe_fd[2]) {
    close(pipe_fd[0]); // Cerrar el lado de lectura del pipe
    for (int i = inicio; i < fin; i++) {
        if (es_primo(i)) {
            write(pipe_fd[1], &i, sizeof(int));
        }
    }
    close(pipe_fd[1]); // Cerrar el lado de escritura del pipe
    exit(0);
}

int main() {
    int pipe_fd[NUM_PROCESOS][2];
    pid_t pids[NUM_PROCESOS];
    int rango_por_proceso = RANGO / NUM_PROCESOS;

    // Crear pipes y procesos hijos
    for (int i = 0; i < NUM_PROCESOS; i++) {
        if (pipe(pipe_fd[i]) == -1) {
            perror("pipe");
            exit(1);
        }

        pids[i] = fork();

        if (pids[i] == 0) { // Proceso hijo
            close(pipe_fd[i][0]); // Cerrar el lado de lectura del pipe
            int inicio = i * rango_por_proceso;
            int fin = (i + 1) * rango_por_proceso;
            encontrar_primos(inicio, fin, pipe_fd[i]);
        } else if (pids[i] < 0) {
            perror("fork");
            exit(1);
        } else { // Proceso padre
            close(pipe_fd[i][1]); // Cerrar el lado de escritura del pipe
        }
    }

    // Leer los resultados de los pipes
    int primos[RANGO] = {0};
    for (int i = 0; i < NUM_PROCESOS; i++) {
        int primo;
        while (read(pipe_fd[i][0], &primo, sizeof(int)) > 0) {
            primos[primo] = primo;
        }
        close(pipe_fd[i][0]); // Cerrar el lado de lectura del pipe
    }

    // Esperar a que todos los procesos hijos terminen
    for (int i = 0; i < NUM_PROCESOS; i++) {
        waitpid(pids[i], NULL, 0);
    }

    printf("Números primos en el rango 0-%d:\n", RANGO - 1);
    for (int i = 0; i < RANGO; i++) {
        if (primos[i] != 0) {
            printf("%d ", primos[i]);
        }
    }
    printf("\n");

    return 0;
}
