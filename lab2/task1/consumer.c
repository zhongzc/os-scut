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

struct queue *q;
sem_t *sem_queue_len;
sem_t *sem_queue_mutex;

void setUp();
void cleanUp();
void nap();

int main(int argc, char const *argv[]) {
  setUp();

  for (;;) {
    nap();
    down(sem_queue_len);
    down(sem_queue_mutex);

    if (q->size == 0 /* queue is empty */) {
      if (q->done == PRODUCER_CNT /* all done */) {
        up(sem_queue_mutex);
        up(sem_queue_len);  // inform other consumers
        break;              // quit
      } else {
        up(sem_queue_mutex);
        continue;
      }
    }

    /* consume a product */
    char c = q->buf[q->con_idx];               // fetch the product
    q->con_idx = (q->con_idx + 1) % BUF_SIZE;  // update product index
    q->size--;                                 // update product count
    printf(MSG, argv[1], c, q->size);

    up(sem_queue_mutex);
  }

  cleanUp();
  return 0;
}

void nap() { usleep(rand() % 100000 + 100000 * FULL); }

void setUp() {
  // fetch shared memory
  key_t semkey = ftok(FTOK_PATH, FTOK_ID);
  int shmid = shmget(semkey, sizeof(struct queue), PERM);
  q = shmat(shmid, NULL, 0);

  // fetch semaphore
  sem_queue_len = sem_open(SEM_QUEUE_LEN_NAME, O_RDWR);
  sem_queue_mutex = sem_open(SEM_QUEUE_MUTEX_NAME, O_RDWR);
}

void cleanUp() {
  // close shared memory and semaphore
  sem_close(sem_queue_len);
  sem_close(sem_queue_mutex);
  shmdt(q);
}