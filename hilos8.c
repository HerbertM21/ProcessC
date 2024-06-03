#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_PROCESOS 4
#define FILAS 4
#define COLUMNAS 4

int A[FILAS][COLUMNAS] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}
};

int B[FILAS][COLUMNAS] = {
    {16, 15, 14, 13},
    {12, 11, 10, 9},
    {8, 7, 6, 5},
    {4, 3, 2, 1}
};

void sumar_matrices(int inicio, int pipe_fd[2]) {
    int C[FILAS][COLUMNAS] = {0};
    int fin = inicio + FILAS / NUM_PROCESOS;

    for (int i = inicio; i < fin; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }

    close(pipe_fd[0]); // Cerrar el lado de lectura
    write(pipe_fd[1], C + inicio, (fin - inicio) * COLUMNAS * sizeof(int)); // Enviar la parte calculada de la matriz
    close(pipe_fd[1]); // Cerrar el lado de escritura
}

int main() {
    int pipe_fd[NUM_PROCESOS][2];
    pid_t pids[NUM_PROCESOS];

    // Crear pipes y procesos hijos
    for (int i = 0; i < NUM_PROCESOS; i++) {
        if (pipe(pipe_fd[i]) == -1) {
            perror("pipe");
            exit(1);
        }

        pids[i] = fork();

        if (pids[i] == 0) { // Proceso hijo
            close(pipe_fd[i][0]); // Cerrar el lado de lectura del pipe
            int inicio = i * (FILAS / NUM_PROCESOS);
            sumar_matrices(inicio, pipe_fd[i]);
            exit(0);
        } else if (pids[i] < 0) {
            perror("fork");
            exit(1);
        } else { // Proceso padre
            close(pipe_fd[i][1]); // Cerrar el lado de escritura del pipe
        }
    }

    int C[FILAS][COLUMNAS] = {0};

    // Leer los resultados de los pipes
    for (int i = 0; i < NUM_PROCESOS; i++) {
        int inicio = i * (FILAS / NUM_PROCESOS);
        int fin = inicio + FILAS / NUM_PROCESOS;
        read(pipe_fd[i][0], C + inicio, (fin - inicio) * COLUMNAS * sizeof(int));
        close(pipe_fd[i][0]); // Cerrar el lado de lectura del pipe
    }

    // Esperar a que todos los procesos hijos terminen
    for (int i = 0; i < NUM_PROCESOS; i++) {
        waitpid(pids[i], NULL, 0);
    }

    printf("Matriz C (resultado de la suma de A y B):\n");
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    return 0;
}
