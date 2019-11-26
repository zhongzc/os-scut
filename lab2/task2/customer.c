#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "barber.h"
#include "common.h"

extern struct customers csm;
extern sem_t *sem_barber_wakeup;
extern sem_t *sem_customers_mutex;

void *customer(void *p) {
  int id = *(int *)p;

  down(sem_customers_mutex);
  if (csm.size == MAX_LEN /* too many customers */) {
    printf(C_GONE_MSG, id);
    up(sem_customers_mutex);

  } else if (csm.size == 0 /* no customer */) {
    csm.size++;
    printf(C_WAKE_MSG, id);
    up(sem_barber_wakeup);  // inform barber to wake up
    up(sem_customers_mutex);

  } else {
    csm.size++;  // update customer count
    printf(C_COME_MSG, id, csm.size);
    up(sem_customers_mutex);
  }

  /* done */
  down(sem_customers_mutex);
  csm.done++;
  up(sem_customers_mutex);

  return NULL;
}