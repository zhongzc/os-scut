#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "common.h"

int idx;

struct rw_table *rwt;

sem_t *sem_readable;
sem_t *sem_writable;
sem_t *sem_table_mutex;

void setUp(char const *argv[]);
void cleanUp();

int main(int argc, char const *argv[]) {
  setUp(argv);

  usleep(rand() % 1000000 + idx * 100000);
  printf(R_COME_MSG, idx);

  down(sem_table_mutex);
  while (rwt->wt.w || rwt->at.w) /* encounter writers */ {
    /* update waiting reader count */
    rwt->wt.r++;
    printf(R_WAIT_MSG, idx, rwt->wt.r - 1, rwt->wt.r, rwt->wt.w);
    up(sem_table_mutex);

    /* wait for notification */
    down(sem_readable);

    down(sem_table_mutex);
    /* update waiting reader count */
    rwt->wt.r--;

    if (rwt->wt.r > 0 && rwt->wt.w == 0) /* other readers are waiting */ {
      up(sem_readable);  // inform them
    }

    printf(R_NOTW_MSG, idx, rwt->wt.r + 1, rwt->wt.r, rwt->wt.w);
  }

  /* update accessing reader count */
  rwt->at.r++;
  printf(R_ACCE_MSG, idx, rwt->at.r - 1, rwt->at.r, rwt->at.w);
  up(sem_table_mutex);

  usleep(rand() % 300000);  // do something ...

  down(sem_table_mutex);
  /* update accessing reader count */
  rwt->at.r--;
  printf(R_GONE_MSG, idx, rwt->at.r + 1, rwt->at.r, rwt->at.w);

  if (rwt->wt.w > 0 && rwt->at.r == 0) /* here the last reader,
                                          and there are waiting
                                          writer */
  {
    up(sem_writable);  // inform a writer
  }

  up(sem_table_mutex);

  cleanUp();
  return 0;
}

void setUp(char const *argv[]) {
  // seed the random
  idx = atoi(argv[1]);
  srand(idx * time(NULL));

  // shared memory fetching
  key_t semkey = ftok(FTOK_PATH, FTOK_ID);
  int shmid = shmget(semkey, sizeof(struct rw_table), IPC_CREAT | PERM);
  rwt = shmat(shmid, NULL, 0);

  // semaphore fetching
  sem_readable = sem_open(SEM_READABLE_NAME, O_RDWR);
  sem_writable = sem_open(SEM_WRITABLE_NAME, O_RDWR);
  sem_table_mutex = sem_open(SEM_TABLE_MUTEX_NAME, O_RDWR);
}

void cleanUp() {
  // close shared memory and semaphore
  sem_close(sem_readable);
  sem_close(sem_writable);
  sem_close(sem_table_mutex);
  shmdt(rwt);
}