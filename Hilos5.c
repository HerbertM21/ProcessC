#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_HILOS 4
#define TAMANO_ARREGLO 16

int arreglo[TAMANO_ARREGLO] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
int suma_total = 0;
pthread_mutex_t mutex;

void* sumar(void* arg) {
    int inicio = *(int*)arg;
    int fin = inicio + TAMANO_ARREGLO / NUM_HILOS;

    for (int i = inicio; i < fin; i++) {
        pthread_mutex_lock(&mutex);
        suma_total += arreglo[i];
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main() {
    pthread_t hilos[NUM_HILOS];
    int indices[NUM_HILOS];
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_HILOS; i++) {
        indices[i] = i * (TAMANO_ARREGLO / NUM_HILOS);
        pthread_create(&hilos[i], NULL, sumar, &indices[i]);
    }

    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    float promedio = suma_total / (float)TAMANO_ARREGLO;
    printf("Suma total: %d\n", suma_total);
    printf("Promedio: %.2f\n", promedio);
    return 0;
}
