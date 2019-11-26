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

const char *MSG = "producer %2d: put   '%c',   buffer size: %d\n";

struct queue *q;
sem_t *sem_queue_len;
sem_t *sem_queue_mutex;

void setUp();
void cleanUp();
void nap();

int main(int argc, char const *argv[]) {
  int cap = atoi(argv[1]);
  setUp();

  for (int i = 0; i < PRODUCT_CNT; i++) {
    nap();

    down(sem_queue_mutex);
    if (q->size == BUF_SIZE /* queue is full */) {
      up(sem_queue_mutex);
      i--;
      continue;
    }

    static int char_idx = 0;
    char c = (cap ? 'A' : 'a') + char_idx;
    char_idx = (char_idx + 1) % 26;

    /* Produce a product */
    q->buf[q->pro_idx] = c;                    // fill the product
    q->pro_idx = (q->pro_idx + 1) % BUF_SIZE;  // update product index
    q->size++;                                 // update product count
    printf(MSG, cap, c, q->size);

    up(sem_queue_mutex);
    up(sem_queue_len);
  }

  /* done */
  down(sem_queue_mutex);
  q->done++;
  up(sem_queue_mutex);

  /* inform the consumers */
  up(sem_queue_len);

  cleanUp();
  return 0;
}

void nap() { usleep(rand() % 100000); }

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