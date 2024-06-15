#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5

pthread_mutex_t palillos[NUM_FILOSOFOS];

void* filosofo(void* num) {
    int id = *(int*)num;
    int izquierda = id;
    int derecha = (id + 1) % NUM_FILOSOFOS; // ulitmo filosofo

    while (1) {
        // Filósofo piensa
        printf("Filósofo %d está pensando.\n", id);
        sleep(rand() % 3 + 1); // Piensa por un tiempo aleatorio

        // Filósofo intenta coger los palillos
        printf("Filósofo %d está hambriento.\n", id);

        // Cogemos los palillos en orden para evitar interbloqueo
        if (id % 2 == 0) {
            pthread_mutex_lock(&palillos[izquierda]);
            pthread_mutex_lock(&palillos[derecha]);
        } else {
            pthread_mutex_lock(&palillos[derecha]);
            pthread_mutex_lock(&palillos[izquierda]);
        }

        // Filósofo come
        printf("Filósofo %d está comiendo.\n", id);
        sleep(rand() % 3 + 1); // Come por un tiempo aleatorio

        // Filósofo deja los palillos
        pthread_mutex_unlock(&palillos[izquierda]);
        pthread_mutex_unlock(&palillos[derecha]);

        printf("Filósofo %d ha terminado de comer.\n", id);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t filosofos[NUM_FILOSOFOS];
    int ids[NUM_FILOSOFOS];

    // Inicializamos los mutexes para los palillos
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_mutex_init(&palillos[i], NULL);
    }

    // Creamos los hilos de los filósofos
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        ids[i] = i;
        pthread_create(&filosofos[i], NULL, filosofo, &ids[i]);
    }

    // Esperamos a que los hilos terminen (en este caso, no lo harán)
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_join(filosofos[i], NULL);
    }

    // Destruimos los mutexes
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_mutex_destroy(&palillos[i]);
    }

    return 0;
}