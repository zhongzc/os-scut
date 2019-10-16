#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include "common.h"

int main(int argc, char const *argv[]) {
  // fetch shared memory
  key_t semkey = ftok(FTOK_PATH, FTOK_ID);
  int shmid = shmget(semkey, sizeof(struct consumers), PERM);
  struct consumers *csm = shmat(shmid, NULL, 0);

  // fetch semaphore
  sem_t *sem_barber_wakeup = sem_open(SEM_BARBER_WAKEUP_NAME, O_RDWR);
  sem_t *sem_consumers_mutex = sem_open(SEM_CONSUMERS_MUTEX_NAME, O_RDWR);

  printf("Barber   %d: sleeping\n", getpid());
  sem_wait(sem_barber_wakeup);
  printf("Barber   %d: wake up\n", getpid());

  for (;;) {
    usleep(500000);
    sem_wait(sem_consumers_mutex);

    printf("Barber   %d: serving 1 consumer, remaining %d\n", getpid(), --csm->size);
    if (csm->size == 0) {
      // done
      if (csm->done == CONSUMER_CNT) {
        sem_post(sem_consumers_mutex);
        break;
      }

      printf("Barber   %d: go to sleep...\n", getpid());
      sem_post(sem_consumers_mutex);
      sem_wait(sem_barber_wakeup);
      printf("Barber   %d: wake up\n", getpid());
    } else {
      sem_post(sem_consumers_mutex);
    }
  }

  // close shared memory and semaphore
  sem_close(sem_barber_wakeup);
  sem_close(sem_consumers_mutex);
  shmdt(csm);
  return 0;
}
