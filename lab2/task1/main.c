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

void exec_producer(int i) {
  if (fork() == 0) {
    execl(PRODUCER, PRODUCER, i ? "1" : "0", 0);
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
  int shmid = shmget(semkey, sizeof(struct queue), IPC_CREAT | PERM);
  struct queue *q = shmat(shmid, NULL, 0);

  // semaphore allocating
  sem_t *sem_queue_len = sem_open(SEM_QUEUE_LEN_NAME, O_CREAT, PERM, SEM_QUEUE_LEN_INIT_VALUE);
  sem_t *sem_queue_mutex = sem_open(SEM_QUEUE_MUTEX_NAME, O_CREAT, PERM, SEM_QUEUE_MUTEX_INIT_VALUE);
  sem_close(sem_queue_len);
  sem_close(sem_queue_mutex);

  // init queue
  q->size = q->pro_idx = q->con_idx = q->done = 0;
  memset(q->buf, 0, BUF_SIZE);
  shmdt(q);

  // exec producer and consumer program
  for (int i = 0; i < PRODUCER_CNT; i++) exec_producer(i);
  for (int i = 0; i < CONSUMER_CNT; i++) exec_consumer();
  for (int i = 0; i < PRODUCER_CNT + CONSUMER_CNT; i++) wait(NULL);

  // release shared memory and semaphore
  sem_unlink(SEM_QUEUE_MUTEX_NAME);
  sem_unlink(SEM_QUEUE_LEN_NAME);
  shmctl(shmid, IPC_RMID, NULL);
  return 0;
}