#ifndef FILE_COMMON
#define FILE_COMMON
#include <semaphore.h>

void up(sem_t *s) { sem_post(s); }

void down(sem_t *s) { sem_wait(s); }

const int PRODUCT_CNT = 30;

const char *const PRODUCER = "./producer";
const int PRODUCER_CNT = 2;

const char *const CONSUMER = "./consumer";
const int CONSUMER_CNT = 3;

const int PERM = 0640;

const char *const FTOK_PATH = ".";
const int FTOK_ID = 42;

const char *const SEM_QUEUE_LEN_NAME = "/queue";
const int SEM_QUEUE_LEN_INIT_VALUE = 0;

const char *const SEM_QUEUE_MUTEX_NAME = "/queue_mutex";
const int SEM_QUEUE_MUTEX_INIT_VALUE = 1;

#define BUF_SIZE (32 - sizeof(int) * 4)
struct queue {
  int size;
  int pro_idx;
  int con_idx;
  char buf[BUF_SIZE];
  int done;
};

const int FULL = 1;

#endif /* !FILE_COMMON */