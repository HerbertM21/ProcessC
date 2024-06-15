#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_ITERATIONS 5

sem_t sem_A, sem_B, sem_C;

void *proceso_A(void *arg) {
    int i;
    for (i = 0; i < NUM_ITERATIONS; ++i) {
        sem_wait(&sem_A);
        printf("A");
        sem_post(&sem_B);
    }
    pthread_exit(NULL);
}

void *proceso_B(void *arg) {
    int i;
    for (i = 0; i < NUM_ITERATIONS; ++i) {
        sem_wait(&sem_B);
        printf("B");
        sem_post(&sem_C);
    }
    pthread_exit(NULL);
}

void *proceso_C(void *arg) {
    int i;
    for (i = 0; i < NUM_ITERATIONS; ++i) {
        sem_wait(&sem_C);
        printf("C ");
        sem_post(&sem_A);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t hilo_A, hilo_B, hilo_C;

    sem_init(&sem_A, 0, 1);  // sem_A se inicializa en 1 (permite que proceso A comience primero)
    sem_init(&sem_B, 0, 0);  // sem_B se inicializa en 0 (proceso B debe esperar a proceso A)
    sem_init(&sem_C, 0, 0);  // sem_C se inicializa en 0 (proceso C debe esperar a proceso B)

    pthread_create(&hilo_A, NULL, proceso_A, NULL);
    pthread_create(&hilo_B, NULL, proceso_B, NULL);
    pthread_create(&hilo_C, NULL, proceso_C, NULL);

    pthread_join(hilo_A, NULL);
    pthread_join(hilo_B, NULL);
    pthread_join(hilo_C, NULL);

    sem_destroy(&sem_A);
    sem_destroy(&sem_B);
    sem_destroy(&sem_C);

    printf("\n");

    return 0;
}
