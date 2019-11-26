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
#include "barber.h"
#include "common.h"
#include "customer.h"

struct customers csm;
sem_t *sem_customers_mutex;
sem_t *sem_barber_wakeup;

int main(void) {
  // set up semaphores
  sem_customers_mutex = sem_open(SEM_CUSTOMERS_MUTEX_NAME, O_CREAT, PERM,
                                 SEM_CUSTOMERS_MUTEX_INIT_VALUE);
  sem_barber_wakeup = sem_open(SEM_BARBER_WAKEUP_NAME, O_CREAT, PERM,
                               SEM_BARBER_WAKEUP_INIT_VALUE);

  // init share variable
  csm.size = csm.done = 0;

  // spawn barber thread
  pthread_t barber_th;
  pthread_create(&barber_th, NULL, barber, NULL);

  // spawn customer threads
  pthread_t cons[CUSTOMER_CNT];
  for (int i = 0; i < CUSTOMER_CNT; i++) {
    usleep(rand() % 500000 + 300000 * FREE);
    pthread_create(&cons[i], NULL, customer, &i);
  }

  // wait for the threads to terminate
  pthread_join(barber_th, NULL);
  for (int i = 0; i < CUSTOMER_CNT; i++) {
    pthread_join(cons[i], NULL);
  }

  // release semaphores
  sem_close(sem_customers_mutex);
  sem_close(sem_barber_wakeup);
  sem_unlink(SEM_CUSTOMERS_MUTEX_NAME);
  sem_unlink(SEM_BARBER_WAKEUP_NAME);
  return 0;
}
