#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "common.h"

void exec_barber() {
  if (fork() == 0) {
    execl(BARBER, BARBER, 0);
  }
}

void exec_consumer() {
  if (fork() == 0) {
    execl(CONSUMER, CONSUMER, 0);
  }
}

int main(void) {
  // shared memory allocating
  key_t semkey = ftok(FTOK_PATH, FTOK_ID);
  int shmid = shmget(semkey, sizeof(struct consumers), IPC_CREAT | PERM);
  shmctl(shmid, IPC_RMID, NULL);
  shmid = shmget(semkey, sizeof(struct consumers), IPC_CREAT | PERM);
  struct consumers *csm = shmat(shmid, NULL, 0);

  // semaphore allocating
  sem_t *sem_consumers_mutex = sem_open(SEM_CONSUMERS_MUTEX_NAME, O_CREAT, PERM, SEM_CONSUMERS_MUTEX_INIT_VALUE);
  sem_t *sem_barber_wakeup = sem_open(SEM_BARBER_WAKEUP_NAME, O_CREAT, PERM, SEM_BARBER_WAKEUP_INIT_VALUE);
  sem_close(sem_consumers_mutex);
  sem_close(sem_barber_wakeup);

  // init length of consumer waiting queue
  csm->size = csm->done = 0;
  shmdt(csm);

  // exec producer and consumer program
  exec_barber();
  for (int i = 0; i < CONSUMER_CNT; i++) {
    usleep(rand() % 500000 + 300000 * FREE);
    exec_consumer();
  }
  for (int i = 0; i < CONSUMER_CNT + 1; i++) wait(NULL);

  // release shared memory and semaphore
  sem_unlink(SEM_CONSUMERS_MUTEX_NAME);
  sem_unlink(SEM_BARBER_WAKEUP_NAME);
  shmctl(shmid, IPC_RMID, NULL);
  return 0;
}