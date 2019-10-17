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

  sem_wait(sem_consumers_mutex);
  if (csm->size == MAX_LEN) {
    printf(C_GONE_MSG, getpid());
    sem_post(sem_consumers_mutex);
  } else if (csm->size == 0) {
    csm->size++;
    printf(C_WAKE_MSG, getpid());
    sem_post(sem_consumers_mutex);
    sem_post(sem_barber_wakeup);
  } else {
    csm->size++;
    printf(C_COME_MSG, getpid(), csm->size);
    sem_post(sem_consumers_mutex);
  }
  sem_wait(sem_consumers_mutex);
  csm->done++;
  sem_post(sem_consumers_mutex);

  // close shared memory and semaphore
  sem_close(sem_barber_wakeup);
  sem_close(sem_consumers_mutex);
  shmdt(csm);
  return 0;
}
