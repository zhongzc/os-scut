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
  printf(W_COME_MSG, idx);
  down(sem_table_mutex);
  if (rwt->wt.w || rwt->at.r || rwt->at.w) /* encounter accessors or
                                              other writers */
  {
    /* update waiting writer count */
    rwt->wt.w++;
    printf(W_WAIT_MSG, idx, rwt->wt.r, rwt->wt.w - 1, rwt->wt.w);
    up(sem_table_mutex);

    /* waiting for notification */
    down(sem_writable);

    down(sem_table_mutex);
    /* update waiting writer count */
    rwt->wt.w--;
    printf(W_NOTW_MSG, idx, rwt->wt.r, rwt->wt.w + 1, rwt->wt.w);
  }
  /* update accessing writer count */
  rwt->at.w++;
  printf(W_ACCE_MSG, idx, rwt->at.r, rwt->at.w - 1, rwt->at.w);
  up(sem_table_mutex);

  usleep(rand() % 300000);  // do something ...

  down(sem_table_mutex);
  /* update accessing writer count */
  rwt->at.w--;
  printf(W_GONE_MSG, idx, rwt->at.r, rwt->at.w + 1, rwt->at.w);

  if (rwt->wt.w > 0) /* there are other waiting writers */ {
    up(sem_writable);  // writer priority
  } else if (rwt->wt.r > 0) /* there are waiting readers */ {
    up(sem_readable);
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