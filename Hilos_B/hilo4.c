//Ejercicio 4: Uso de variables compartidas y mutex
//Objetivo: Crear hilos que incrementen una variable compartida de manera segura usando un mutex.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_HILOS 8
#define NUM_INCREMENTOS 1000000

int contador = 0;
pthread_mutex_t mutex;

void* hilo_funcion(void* arg) {
    for (int i = 0; i < NUM_INCREMENTOS; i++) {
        pthread_mutex_lock(&mutex);
        // Código crítico
        contador++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t hilos[NUM_HILOS];
    pthread_mutex_init(&mutex, NULL);

    // Crear los hilos
    for (int i = 0; i < NUM_HILOS; i++) {
        if (pthread_create(&hilos[i], NULL, hilo_funcion, NULL)) {
            printf("Error al crear el hilo %d\n", i);
            return 1; // Return 1 porque es un error al crear el hilo
        }
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < NUM_HILOS; i++) {
        if (pthread_join(hilos[i], NULL)) {
            printf("Error al esperar el hilo %d\n", i);
            return 2; // Return 2 porque es un error distinto al de crear el hilo
        }
    }

    pthread_mutex_destroy(&mutex);
    printf("Contador final: %d\n", contador);
    return 0;
}
