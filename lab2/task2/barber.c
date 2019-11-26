#include "barber.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"

extern struct customers csm;
extern sem_t *sem_barber_wakeup;
extern sem_t *sem_customers_mutex;

void *barber(void *_) {
  printf("%s", B_SLEEP_MSG);
  down(sem_barber_wakeup);
  printf("%s", B_WAKE_MSG);

  for (;;) {
    usleep(500000);
    down(sem_customers_mutex);

    // serve a customer
    printf(B_SERVE_MSG, --csm.size);

    if (csm.size == 0 /* waiting queue is empty */) {
      if (csm.done == CUSTOMER_CNT /* done */) {
        up(sem_customers_mutex);
        break;
      }

      printf("%s", B_SLEEP_MSG);  // go to sleep
      up(sem_customers_mutex);
      
      down(sem_barber_wakeup);  // wait for informing
      printf("%s", B_WAKE_MSG);
    } else {
      up(sem_customers_mutex);
    }
  }
  return NULL;
}