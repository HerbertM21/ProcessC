#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

#define MAX_BLOCKED_IPS 100

char blocked_ips[MAX_BLOCKED_IPS][16];
int num_blocked_ips = 0;

void sigchld_handler(int sig) {
    int status;
    waitpid(-1, &status, WNOHANG);
}

void sigint_handler(int sig) {
    printf("\nSe recibió la señal SIGINT. Terminando el servidor...\n");
    exit(0);
}

void block_ip(char *ip) {
    if (num_blocked_ips >= MAX_BLOCKED_IPS) {
        printf("Límite máximo de IPs bloqueadas alcanzado\n");
        return;
    }

    strcpy(blocked_ips[num_blocked_ips], ip);
    num_blocked_ips++;
    printf("IP %s bloqueada\n", ip);
}

int is_ip_blocked(char *ip) {
    for (int i = 0; i < num_blocked_ips; i++) {
        if (strcmp(blocked_ips[i], ip) == 0) {
            return 1;
        }
    }
    return 0;
}

void handle_connection(char *client_ip) {
    if (is_ip_blocked(client_ip)) {
        printf("Conexión rechazada de IP bloqueada: %s\n", client_ip);
        return;
    }

    printf("Conexión entrante aceptada de IP: %s\n", client_ip);

    // Simulación de manejo de conexión
    sleep(5);

    printf("Conexión con IP %s finalizada\n", client_ip);
}

int main() {
    signal(SIGCHLD, sigchld_handler);
    signal(SIGINT, sigint_handler);

    while (1) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("Error al crear proceso hijo");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Proceso hijo
            char client_ip[] = "192.168.1.100"; // Simular una IP de cliente
            handle_connection(client_ip);
            exit(EXIT_SUCCESS);
        } else {
            // Proceso padre
            // Simular detección de intento de acceso no autorizado
            block_ip("10.0.0.5");
        }
    }

    return 0;
}

// Lo que hace este metodo es ejecutar dos bloques de codigo simultaneamente
// y no de forma lineal como se hace en el llamado de funciones normalmente en los codigos.