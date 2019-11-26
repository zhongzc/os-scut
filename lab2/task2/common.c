#include "common.h"

void up(sem_t *s) {
    sem_post(s);
}
void down(sem_t *s) {
    sem_wait(s);
}

int MAX_LEN = 7;

char *BARBER = "./barber";
char *CUSTOMER = "./customer";
int CUSTOMER_CNT = 20;

int PERM = 0640;
char *SEM_CUSTOMERS_MUTEX_NAME = "/customers_mutex";
int SEM_CUSTOMERS_MUTEX_INIT_VALUE = 1;
char *SEM_BARBER_WAKEUP_NAME = "/barber_wakeup";
int SEM_BARBER_WAKEUP_INIT_VALUE = 0;

int FREE = 1;
char *C_COME_MSG = "Customer %2d: coming a new customer, there are %d now\n";
char *C_GONE_MSG = "Customer %2d: too many customers, gone\n";
char *C_WAKE_MSG = "Customer %2d: a new customer to wake up the barber\n";
char *B_SERVE_MSG = "Barber     : serving 1 customer, remaining %d\n";
char *B_SLEEP_MSG = "Barber     : go to sleep...\n";
char *B_WAKE_MSG = "Barber     : wake up\n";