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

void *barber(void *);
void *consumer(void *);

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

void *barber(void *_) {
  printf("%s", B_SLEEP_MSG);
  sem_wait(sem_barber_wakeup);
  printf("%s", B_WAKE_MSG);

  for (;;) {
    usleep(500000);
    sem_wait(sem_consumers_mutex);

    printf(B_SERVE_MSG, --csm.size);
    if (csm.size == 0) {
      // done
      if (csm.done == CONSUMER_CNT) {
        sem_post(sem_consumers_mutex);
        break;
      }

      printf("%s", B_SLEEP_MSG);
      sem_post(sem_consumers_mutex);
      sem_wait(sem_barber_wakeup);
      printf("%s", B_WAKE_MSG);
    } else {
      sem_post(sem_consumers_mutex);
    }
  }
  return NULL;
}

void *consumer(void *p) {
  int id = *(int *)p;
  sem_wait(sem_consumers_mutex);
  if (csm.size == MAX_LEN) {
    printf(C_GONE_MSG, id);
    sem_post(sem_consumers_mutex);
  } else if (csm.size == 0) {
    csm.size++;
    printf(C_WAKE_MSG, id);
    sem_post(sem_consumers_mutex);
    sem_post(sem_barber_wakeup);
  } else {
    csm.size++;
    printf(C_COME_MSG, id, csm.size);
    sem_post(sem_consumers_mutex);
  }
  sem_wait(sem_consumers_mutex);
  csm.done++;
  sem_post(sem_consumers_mutex);
  return NULL;
}