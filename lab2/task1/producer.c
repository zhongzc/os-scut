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
  int cap = atoi(argv[1]);

  // fetch shared memory
  key_t semkey = ftok(FTOK_PATH, FTOK_ID);
  int shmid = shmget(semkey, sizeof(struct queue), PERM);
  struct queue *q = shmat(shmid, NULL, 0);

  // fetch semaphore
  sem_t *sem_queue_len = sem_open(SEM_QUEUE_LEN_NAME, O_RDWR);
  sem_t *sem_queue_mutex = sem_open(SEM_QUEUE_MUTEX_NAME, O_RDWR);

  for (int i = 0; i < PRODUCER_TIMES; i++) {
    usleep(rand() % 100000);

    sem_wait(sem_queue_mutex);
    if (q->size == BUF_SIZE) {
      sem_post(sem_queue_mutex);
      i--;
      continue;
    }

    static int char_idx = 0;
    char c = (cap ? 'A' : 'a') + char_idx;
    char_idx = (char_idx + 1) % 26;

    q->size++;
    q->buf[q->pro_idx] = c;
    q->pro_idx = (q->pro_idx + 1) % BUF_SIZE;
    printf("producer %d: put   '%c',   buffer size: %d\n", getpid(), c, q->size);

    sem_post(sem_queue_mutex);
    sem_post(sem_queue_len);
  }

  // close shared memory and semaphore
  sem_close(sem_queue_len);
  sem_close(sem_queue_mutex);
  shmdt(q);
  return 0;
}
