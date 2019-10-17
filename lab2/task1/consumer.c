#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include "common.h"

const char *MSG = "consumer %s: fetch '%c',   buffer size: %d\n";

int main(int argc, char const *argv[]) {
  const char *p_name = argv[1];

  // fetch shared memory
  key_t semkey = ftok(FTOK_PATH, FTOK_ID);
  int shmid = shmget(semkey, sizeof(struct queue), PERM);
  struct queue *q = shmat(shmid, NULL, 0);

  // fetch semaphore
  sem_t *sem_queue_len = sem_open(SEM_QUEUE_LEN_NAME, O_RDWR);
  sem_t *sem_queue_mutex = sem_open(SEM_QUEUE_MUTEX_NAME, O_RDWR);

  for (;;) {
    usleep(rand() % 100000 + 100000 * FULL);
    sem_wait(sem_queue_len);
    sem_wait(sem_queue_mutex);

    // queue is empty
    if (q->size == 0) {
      if (q->done == PRODUCER_CNT) {
        // done
        sem_post(sem_queue_mutex);
        sem_post(sem_queue_len);
        break;
      } else {
        sem_post(sem_queue_mutex);
        continue;
      }
    }

    // consume a product
    q->size--;
    char c = q->buf[q->con_idx];
    q->con_idx = (q->con_idx + 1) % BUF_SIZE;
    printf(MSG, p_name, c, q->size);

    sem_post(sem_queue_mutex);
  }

  // close shared memory and semaphore
  sem_close(sem_queue_len);
  sem_close(sem_queue_mutex);
  shmdt(q);
  return 0;
}
