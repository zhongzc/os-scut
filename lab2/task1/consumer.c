#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include "common.h"

int main(int argc, char const *argv[]) {
  // fetch shared memory
  key_t semkey = ftok(FTOK_PATH, FTOK_ID);
  int shmid = shmget(semkey, sizeof(struct queue), PERM);
  struct queue *q = shmat(shmid, NULL, 0);

  // fetch semaphore
  sem_t *sem_queue_len = sem_open(SEM_QUEUE_LEN_NAME, O_RDWR);
  sem_t *sem_queue_mutex = sem_open(SEM_QUEUE_MUTEX_NAME, O_RDWR);

  for (int i = 0; i < CONSUMER_TIMES; i++) {
    usleep(rand() % 100000 + 100000);

    sem_wait(sem_queue_len);
    sem_wait(sem_queue_mutex);

    if (q->size == 0) {
      sem_post(sem_queue_mutex);
      i--;
      continue;
    }
    q->size--;
    char c = q->buf[q->con_idx];
    q->con_idx = (q->con_idx + 1) % BUF_SIZE;
    printf("consumer %d: fetch '%c',   buffer size: %d\n", getpid(), c, q->size);

    sem_post(sem_queue_mutex);
  }

  // close shared memory and semaphore
  sem_close(sem_queue_len);
  sem_close(sem_queue_mutex);
  shmdt(q);
  return 0;
}
