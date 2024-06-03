#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_HILOS 4
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

int C[FILAS][COLUMNAS] = {0};
pthread_mutex_t mutex;

void* sumar_matrices(void* arg) {
    int inicio = *(int*)arg;
    int fin = inicio + FILAS / NUM_HILOS;

    for (int i = inicio; i < fin; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            pthread_mutex_lock(&mutex);
            C[i][j] = A[i][j] + B[i][j];
            pthread_mutex_unlock(&mutex);
        }
    }

    return NULL;
}
int main() {
    pthread_t hilos[NUM_HILOS];
    int indices[NUM_HILOS];
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_HILOS; i++) {
        indices[i] = i * (FILAS / NUM_HILOS);
        pthread_create(&hilos[i], NULL, sumar_matrices, &indices[i]);
    }

    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("Matriz C (resultado de la suma de A y B):\n");
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    return 0;
}
