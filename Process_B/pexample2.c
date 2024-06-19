
#include <stdio.h>    // Standard I/O
#include <unistd.h>   // Unix Standard
#include <sys/wait.h> // System Wait
#include <stdlib.h>   // Standard library

int main(int argc,char** argv) {
  int pid, status, number = 300;
  // Create a process (fork)
  if ((pid=fork())==0) {
    // Child process
    printf("Child process: Number is %d\n",number);
    number = 400;
    // Terminate OK
    exit(0);
  } else {
    // Father process
    printf("Father process: Number is %d\n",number);
    number = 500;
    // Wait for child to finish
    wait(&status);
  }
  return 0; // Terminate OK
}
