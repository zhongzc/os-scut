#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SEM_NAME "/semaphore_example"
#define ITERS 10

int main(void) {
  sem_t *semaphore = sem_open(SEM_NAME, O_RDWR);
  for (size_t i = 0; i < ITERS; i++) {
    sem_wait(semaphore);
    printf("PID %ld acquired semaphore\n", (long)getpid());
    sem_post(semaphore);

    sleep(1);
  }

  sem_close(semaphore);
}