//Ejercicio 3: Crear múltiples hilos
//Objetivo: Crear varios hilos que impriman su número de identificación.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* hilo_funcion(void* arg) {
    int id = *(int*)arg;
    printf("Hola desde el hilo %d\n", id);
    for(;;);
    return NULL;
}

int main() {
    pthread_t hilos[5];
    int ids[5];

    for (int i = 0; i < 5; i++) {
        ids[i] = i;
        if (pthread_create(&hilos[i], NULL, hilo_funcion, &ids[i])) {
            fprintf(stderr, "Error al crear el hilo %d\n", i);
            return 1;
        }
    }

    for (int i = 0; i < 5; i++) {
        if (pthread_join(hilos[i], NULL)) {
            printf("Error al esperar el hilo %d\n", i);
            return 2;
        }
    }

    printf("Hola desde el hilo principal\n");
    return 0;
}
