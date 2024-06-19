#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_HILOS 4
#define TAMANO_ARREGLO 16
#define OBJETIVO 7

int arreglo[TAMANO_ARREGLO] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
int encontrado = 0;
pthread_mutex_t mutex;

void* buscar(void* arg) {
    int inicio = *(int*)arg;
    int fin = inicio + TAMANO_ARREGLO / NUM_HILOS;

    for (int i = inicio; i < fin; i++) {
        pthread_mutex_lock(&mutex);
        if (arreglo[i] == OBJETIVO) {
            encontrado = 1;
            pthread_mutex_unlock(&mutex);
            break;
        }
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
        pthread_create(&hilos[i], NULL, buscar, &indices[i]);
    }

    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    if (encontrado) {
        printf("Elemento %d encontrado en el arreglo.\n", OBJETIVO);
    } else {
        printf("Elemento %d no encontrado en el arreglo.\n", OBJETIVO);
    }

    return 0;
}
