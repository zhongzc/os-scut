#ifndef FILE_COMMON
#define FILE_COMMON

#include <semaphore.h>

void up(sem_t *);
void down(sem_t *);

int MAX_LEN;

char *BARBER;
char *CUSTOMER;
int CUSTOMER_CNT;

int PERM;

char *SEM_CUSTOMERS_MUTEX_NAME;
int SEM_CUSTOMERS_MUTEX_INIT_VALUE;
char *SEM_BARBER_WAKEUP_NAME;
int SEM_BARBER_WAKEUP_INIT_VALUE;

int FREE;

struct customers {
  int size;
  int done;
};

char *C_COME_MSG;
char *C_GONE_MSG;
char *C_WAKE_MSG;
char *B_SERVE_MSG;
char *B_SLEEP_MSG;
char *B_WAKE_MSG;

#endif /* !FILE_COMMON */
