//Ejercicio 1: Creación de un hilo simple
//Objetivo: Crear un hilo que imprima "Hola desde el hilo".

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* hilo_funcion(void* arg) {
    printf("Hola desde el hilo\n");
    return NULL;
}

int main() {
    pthread_t hilo;
    
    // Crear el hilo
    // int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
    if (pthread_create(&hilo, NULL, hilo_funcion, NULL)!=0) {
        printf("Error al crear el hilo\n");
        return 1;
    }

    // Esperar a que el hilo termine
    // int pthread_join(pthread_t thread, void **retval);
    if (pthread_join(hilo, NULL)!=0) {
        printf("Error al esperar el hilo\n");
        return 2;
    }

    printf("Hola desde el hilo principal\n");
    return 0;
}
