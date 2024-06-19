#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

// REPASO: DESCRIPTOR DE ARCHIVO
// Un descriptor de archivo es un identificador único asignado por el kernel a un 
// recurso abierto por un proceso en un sistema operativo tipo Unix.

// REPASO: PIPE
// Un pipe es un mecanismo de comunicación entre procesos que permite la comunicación
// entre un proceso padre y un proceso hijo. Un pipe es unidireccional, es decir,
// sólo permite la comunicación en un sentido.

int main(void){
  
  pid_t pidA;
  int fd[2];
  char buffer[80];
  int numBytes;

  pipe(fd);

  pidA = fork();

  switch(pidA){
    
    case 0: /* HIJO */
      close(fd[0]); // Cerramos el descriptor de lectura
      write(fd[1], "Hola, soy el hijo\0", 18); // Escribimos en el pipe
      close(fd[1]); // IMPORTANTE: cerrar el pipe antes de salir
      exit(0); 
      break;

    case -1:
        /* ERROR */

    default: /* PADRE */
      close(fd[1]);
      numBytes = read(fd[0], buffer, sizeof(buffer));
      write(1, buffer, numBytes);
      close(fd[0]); // IMPORTANTE: cerrar el pipe antes de salir
      break;
  }

// Los procesos ejecutan el mismo código, pero el valor de retorno de fork() es diferente
// en cada uno de ellos. En el proceso hijo, fork() devuelve 0, mientras que en el proceso
// padre, fork() devuelve el PID del proceso hijo.

// El proceso hijo es una copia exacta del proceso padre, pero con un espacio de memoria
// diferente. El proceso hijo tiene su propio espacio de memoria, y no puede acceder a las
// variables del proceso padre.

// usando WAIT(NULL)
// El proceso padre espera a que el proceso hijo termine de ejecutarse. Si el proceso hijo
// termina de ejecutarse, el proceso padre continúa su ejecución. Si el proceso hijo no
// termina de ejecutarse, el proceso padre se queda esperando.

// En este caso, no hace falta que el padre espere al hijo, dado que
// simultaneamente, el padre lee del pipe y el hijo escribe en el pipe al mismo tiempo.

  return 0;
}
