#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 5

// Buffer que representa la caja de panes
int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

sem_t empty;   // Semáforo para controlar espacios vacíos en el buffer
sem_t full;    // Semáforo para controlar espacios llenos en el buffer
pthread_mutex_t mutex;  // Mutex para garantizar acceso exclusivo al buffer

void* fabrica(void* arg) {
    int pan_id = 0;

    while (1) {
        // Producir un pan (aumentar el identificador del pan)
        int pan = ++pan_id;
        printf("Fábrica: Produciendo pan %d\n", pan);
        sleep(rand() % 3); // Simular tiempo de producción

        sem_wait(&empty);    // Decrementar el semáforo empty (espacios vacíos)
        pthread_mutex_lock(&mutex);  // Bloquear el mutex antes de acceder al buffer

        // Colocar el pan en la caja
        buffer[in] = pan;
        printf("Fábrica: Colocó pan %d en la posición %d\n", pan, in);
        in = (in + 1) % BUFFER_SIZE; // Actualizar la posición del buffer

        pthread_mutex_unlock(&mutex);  // Desbloquear el mutex después de usar el buffer
        sem_post(&full);    // Incrementar el semáforo full (espacios llenos)
    }

    pthread_exit(NULL);
}

void* tienda(void* arg) {
    while (1) {
        sem_wait(&full);    // Decrementar el semáforo full (espacios llenos)
        pthread_mutex_lock(&mutex);  // Bloquear el mutex antes de acceder al buffer

        // Tomar un pan de la caja
        int pan = buffer[out];
        printf("Tienda: Tomó pan %d de la posición %d\n", pan, out);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);  // Desbloquear el mutex después de usar el buffer
        sem_post(&empty);   // Incrementar el semáforo empty (espacios vacíos)

        printf("Tienda: Vendiendo pan %d\n", pan);
        sleep(rand() % 4); // Simular tiempo de venta
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t thread_fabrica, thread_tienda;

    sem_init(&empty, 0, BUFFER_SIZE);   // Inicialización del semáforo empty
    sem_init(&full, 0, 0);              // Inicialización del semáforo full
    pthread_mutex_init(&mutex, NULL);   // Inicialización del mutex

    // Crear hilo para la fábrica
    pthread_create(&thread_fabrica, NULL, fabrica, NULL);

    // Crear hilo para la tienda
    pthread_create(&thread_tienda, NULL, tienda, NULL);

    // Esperar a que los hilos terminen (esto no se ejecutará nunca debido a los bucles infinitos)
    pthread_join(thread_fabrica, NULL);
    pthread_join(thread_tienda, NULL);

    sem_destroy(&empty);   // Destrucción del semáforo empty
    sem_destroy(&full);    // Destrucción del semáforo full
    pthread_mutex_destroy(&mutex);   // Destrucción del mutex

    return 0;
}
