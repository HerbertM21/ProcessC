#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int arreglo[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int suma_pares = 0;
int suma_impares = 0;

void* sumar_pares(void* arg) {
    for (int i = 0; i < 10; i++) {
        if (arreglo[i] % 2 == 0) {
            suma_pares += arreglo[i];
        }
    }
    pthread_exit(NULL); // Termina el hilo
}

void* sumar_impares(void* arg) {
    for (int i = 0; i < 10; i++) {
        if (arreglo[i] % 2 != 0) {
            suma_impares += arreglo[i];
        }
    } 
    pthread_exit(NULL); // Termina el hilo
}

int main(int argc, char** argv) {
    pthread_t hilo_pares;
    pthread_t hilo_impares;

    pthread_create(&hilo_pares, NULL, sumar_pares, NULL); // Se crea el hilo
    pthread_create(&hilo_impares, NULL, sumar_impares, NULL); // Se crea el hilo

    pthread_join(hilo_pares, NULL); // Se espera a que el hilo termine
    pthread_join(hilo_impares, NULL); // Se espera a que el hilo termine

    printf("Suma de pares: %d\n", suma_pares);
    printf("Suma de impares: %d\n", suma_impares);

    return 0;
}
