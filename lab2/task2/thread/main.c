#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "common.h"

struct consumers csm;
sem_t *sem_consumers_mutex;
sem_t *sem_barber_wakeup;

void* barber(void *);
void* consumer(void *);

int main(void) {
  sem_consumers_mutex = sem_open(SEM_CONSUMERS_MUTEX_NAME, O_CREAT, PERM, SEM_CONSUMERS_MUTEX_INIT_VALUE);
  sem_barber_wakeup = sem_open(SEM_BARBER_WAKEUP_NAME, O_CREAT, PERM, SEM_BARBER_WAKEUP_INIT_VALUE);
  csm.size = csm.done = 0;

  pthread_t barber_th;
  pthread_t cons[CONSUMER_CNT];

  pthread_create(&barber_th, NULL, barber, NULL);
  for (int i = 0; i < CONSUMER_CNT; i++) {
    usleep(rand() % 500000 + 300000 * FREE);
    pthread_create(&cons[i], NULL, consumer, &i);
  }
  pthread_join(barber_th, NULL);
  for (int i = 0; i < CONSUMER_CNT; i++) {
    pthread_join(cons[i], NULL);
  }

  // release shared memory and semaphore
  sem_close(sem_consumers_mutex);
  sem_close(sem_barber_wakeup);
  sem_unlink(SEM_CONSUMERS_MUTEX_NAME);
  sem_unlink(SEM_BARBER_WAKEUP_NAME);
  return 0;
}

void* barber(void *_) {
  printf("Barber     : sleeping\n");
  sem_wait(sem_barber_wakeup);
  printf("Barber     : wake up\n");

  for (;;) {
    usleep(500000);
    sem_wait(sem_consumers_mutex);

    printf("Barber     : serving 1 consumer, remaining %d\n", --csm.size);
    if (csm.size == 0) {
      // done
      if (csm.done == CONSUMER_CNT) {
        sem_post(sem_consumers_mutex);
        break;
      }

      printf("Barber     : go to sleep...\n");
      sem_post(sem_consumers_mutex);
      sem_wait(sem_barber_wakeup);
      printf("Barber     : wake up\n");
    } else {
      sem_post(sem_consumers_mutex);
    }
  }
  return NULL;
}

void* consumer(void *p) {
  int id = *(int *)p;
  sem_wait(sem_consumers_mutex);
  if (csm.size == MAX_LEN) {
    printf("Consumer %2d: too many consumers, gone\n", id);
    sem_post(sem_consumers_mutex);
  } else if (csm.size == 0) {
    csm.size++;
    printf("Consumer %2d: a new consumer to wake up the barber\n", id);
    sem_post(sem_consumers_mutex);
    sem_post(sem_barber_wakeup);
  } else {
    csm.size++;
    printf("Consumer %2d: coming a new consumer, there are %d now\n", id, csm.size);
    sem_post(sem_consumers_mutex);
  }
  sem_wait(sem_consumers_mutex);
  csm.done++;
  sem_post(sem_consumers_mutex);
  return NULL;
}