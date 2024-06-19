//Ejercicio 2: Pasar argumentos a un hilo
//Objetivo: Crear un hilo que reciba un argumento e imprima un mensaje personalizado.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* hilo_funcion(void* arg) {
    char* mensaje = (char*)arg;
    printf("%s\n", mensaje);
    return NULL;
}

int main() {
    pthread_t hilo;
    char* mensaje = "Hola, soy un hilo secundario con argumento";

    // Crear el hilo
    if (pthread_create(&hilo, NULL, hilo_funcion, mensaje)) {
        printf("Error al crear el hilo\n");
        return 1;
    }

    // Esperar a que el hilo termine
    if (pthread_join(hilo, NULL)) {
        printf("Error al esperar el hilo\n");
        return 2;
    }

    printf("Hola desde el hilo principal\n");
    return 0;
}
