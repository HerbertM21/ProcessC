#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define NUM_HILOS 4
#define RANGO 100

typedef struct {
    int inicio;
    int fin;
    int* primos;
} ThreadData;

bool es_primo(int num) {
    if (num < 2) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}

void* encontrar_primos(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    for (int i = data->inicio; i < data->fin; i++) {
        if (es_primo(i)) {
            data->primos[i - data->inicio] = i;
        } else {
            data->primos[i - data->inicio] = 0;
        }
    }
    return NULL;
}

int main() {
    pthread_t hilos[NUM_HILOS];
    ThreadData datos_hilos[NUM_HILOS];
    int rango_por_hilo = RANGO / NUM_HILOS;
    int primos[RANGO] = {0};

    for (int i = 0; i < NUM_HILOS; i++) {
        datos_hilos[i].inicio = i * rango_por_hilo;
        datos_hilos[i].fin = (i + 1) * rango_por_hilo;
        datos_hilos[i].primos = primos + i * rango_por_hilo;
        pthread_create(&hilos[i], NULL, encontrar_primos, &datos_hilos[i]);
    }

    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
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
