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

int main(int argc, char const *argv[]) {
  int idx = atoi(argv[1]);
  srand(idx * time(NULL));

  // shared memory fetching
  key_t semkey = ftok(FTOK_PATH, FTOK_ID);
  int shmid = shmget(semkey, sizeof(struct rw_table), IPC_CREAT | PERM);
  struct rw_table *rwt = shmat(shmid, NULL, 0);

  // semaphore fetching
  sem_t *sem_readable = sem_open(SEM_READABLE_NAME, O_RDWR);
  sem_t *sem_writable = sem_open(SEM_WRITABLE_NAME, O_RDWR);
  sem_t *sem_table_mutex = sem_open(SEM_TABLE_MUTEX_NAME, O_RDWR);

  usleep(rand() % 1000000 + idx * 100000);
  printf("Reader %3d: a reader is coming\n", idx);
  sem_wait(sem_table_mutex);
  if (rwt->waiting_table.writer || rwt->accessing_table.writer) {
    rwt->waiting_table.reader++;
    sem_post(sem_table_mutex);
    printf("Reader %3d: waiting             (waiting-reader: %2d -> %2d, waiting-writer:       %2d)\n",
           idx, rwt->waiting_table.reader-1, rwt->waiting_table.reader, rwt->waiting_table.writer);

    // waiting for notification
    sem_wait(sem_readable);
    sem_wait(sem_table_mutex);
    rwt->waiting_table.reader--;
    if (rwt->waiting_table.reader > 0 && rwt->waiting_table.writer == 0) sem_post(sem_readable);
    printf("Reader %3d: no need to wait     (waiting-reader: %2d -> %2d, waiting-writer:       %2d)\n", 
           idx, rwt->waiting_table.reader+1, rwt->waiting_table.reader, rwt->waiting_table.writer);
  }
  rwt->accessing_table.reader++;
  printf("Reader %3d: accessing           [viewing-reader: %2d -> %2d, viewing-writer:       %2d]\n",
         idx, rwt->accessing_table.reader-1, rwt->accessing_table.reader, rwt->accessing_table.writer);
  sem_post(sem_table_mutex);

  usleep(rand() % 300000);

  sem_wait(sem_table_mutex);
  rwt->accessing_table.reader--;
  printf("Reader %3d: leaving             [viewing-reader: %2d -> %2d, viewing-writer:       %2d]\n",
         idx, rwt->accessing_table.reader+1, rwt->accessing_table.reader, rwt->accessing_table.writer);
  if (rwt->waiting_table.writer > 0 && rwt->accessing_table.reader == 0) sem_post(sem_writable);
  sem_post(sem_table_mutex);

  // close shared memory and semaphore
  sem_close(sem_readable);
  sem_close(sem_writable);
  sem_close(sem_table_mutex);
  shmdt(rwt);
  return 0;
}
