#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "common.h"

void exec_reader(int i) {
  if (fork() == 0) {
    char idx[4];
    sprintf(idx, "%d", i);
    execl(READER, READER, idx, 0);
  }
}

void exec_writer(int i) {
  if (fork() == 0) {
    char idx[4];
    sprintf(idx, "%d", i);
    execl(WRITER, WRITER, idx, 0);
  }
}

int main(void) {
  // shared memory allocating
  key_t semkey = ftok(FTOK_PATH, FTOK_ID);
  int shmid = shmget(semkey, sizeof(struct rw_table), IPC_CREAT | PERM);
  struct rw_table *rwt = shmat(shmid, NULL, 0);

  // semaphore allocating
  sem_t *sem_readable    = sem_open(SEM_READABLE_NAME, O_CREAT, PERM, SEM_READABLE_INIT_VALUE);
  sem_t *sem_writable    = sem_open(SEM_WRITABLE_NAME, O_CREAT, PERM, SEM_WRITABLE_INIT_VALUE);
  sem_t *sem_table_mutex = sem_open(SEM_TABLE_MUTEX_NAME, O_CREAT, PERM, SEM_TABLE_MUTEX_INIT_VALUE);
  sem_close(sem_readable);
  sem_close(sem_writable);
  sem_close(sem_table_mutex);

  // init length of consumer waiting queue
  rwt->waiting_table.reader = rwt->waiting_table.writer = \
  rwt->accessing_table.reader = rwt->accessing_table.writer = 0;
  shmdt(rwt);

  // exec producer and consumer program
  for (int i = 0; i < READER_CNT; i++) exec_reader(i);
  for (int i = 0; i < WRITER_CNT; i++) exec_writer(i);
  for (int i = 0; i < READER_CNT + WRITER_CNT; i++) wait(NULL);

  // release shared memory and semaphore
  sem_unlink(SEM_READABLE_NAME);
  sem_unlink(SEM_WRITABLE_NAME);
  sem_unlink(SEM_TABLE_MUTEX_NAME);
  shmctl(shmid, IPC_RMID, NULL);
  return 0;
}
