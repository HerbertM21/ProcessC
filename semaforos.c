#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

sem_t empty;   // Semáforo para controlar espacios vacíos en el buffer
sem_t full;    // Semáforo para controlar espacios llenos en el buffer
pthread_mutex_t mutex;  // Mutex para garantizar acceso exclusivo al buffer

void *productor(void *thread_id) {
    int tid = *(int *)thread_id; // *(int *) es un cast que convierte el puntero a void a un puntero a int

    while (1) { 
        sem_wait(&empty);   // Decrementar el semáforo empty (espacios vacíos)
        pthread_mutex_lock(&mutex);  // Bloquear el mutex antes de acceder al buffer
        buffer[in] = tid;   // Colocar el item en el buffer
        printf("Productor %d: Produjo item en posición %d\n", tid, in);
        in = (in + 1) % BUFFER_SIZE; // Actualizar la posición del buffer, incrementando en 1 y volviendo al inicio si se llega al final
        pthread_mutex_unlock(&mutex);  // Desbloquear el mutex después de usar el buffer
        sem_post(&full);    // Incrementar el semáforo full (espacios llenos)
        //sleep(1); Usar sleep para que printee de forma intercalada
    }

    pthread_exit(NULL);
}

void *consumidor(void *thread_id) {
    int tid = *(int *)thread_id;

    while (1) {
        sem_wait(&full);    // Decrementar el semáforo full (espacios llenos)
        pthread_mutex_lock(&mutex);  // Bloquear el mutex antes de acceder al buffer
        int item = buffer[out];   // Consumir un item del buffer
        printf("Consumidor %d: Consumió item en posición %d\n", tid, out);
        out = (out + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);  // Desbloquear el mutex después de usar el buffer
        sem_post(&empty);   // Incrementar el semáforo empty (espacios vacíos)
        // sleep(1);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t productores[NUM_PRODUCERS];
    pthread_t consumidores[NUM_CONSUMERS];
    int prod_ids[NUM_PRODUCERS];
    int cons_ids[NUM_CONSUMERS];

    sem_init(&empty, 0, BUFFER_SIZE);   // Inicialización del semáforo empty
    sem_init(&full, 0, 0);              // Inicialización del semáforo full
    pthread_mutex_init(&mutex, NULL);   // Inicialización del mutex

    // Crear hilos productores
    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        prod_ids[i] = i + 1;
        pthread_create(&productores[i], NULL, productor, (void *)&prod_ids[i]);
    }

    // Crear hilos consumidores
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        cons_ids[i] = i + 1;
        pthread_create(&consumidores[i], NULL, consumidor, (void *)&cons_ids[i]);
    }

    // Esperar a que los hilos terminen (esto no se ejecutará nunca)
    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        pthread_join(productores[i], NULL);
    }
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        pthread_join(consumidores[i], NULL);
    }

    sem_destroy(&empty);   // Destrucción del semáforo empty
    sem_destroy(&full);    // Destrucción del semáforo full
    pthread_mutex_destroy(&mutex);   // Destrucción del mutex

    return 0;
}
