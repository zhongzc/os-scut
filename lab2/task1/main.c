#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "common.h"

void exec_producer() {
  if (fork() == 0) {
    execl(PRODUCER, PRODUCER, 0);
  }
  wait(NULL);
}

void exec_consumer() {
  if (fork() == 0) {
    execl(CONSUMER, CONSUMER, 0);
  }
  wait(NULL);
}

int main(void) {
  key_t semkey = ftok(FTOK_PATH, FTOK_ID);
  int shmid = shmget(semkey, 1024 * sizeof(char), IPC_CREAT | 0660);

  exec_producer();
  exec_consumer();

  shmctl(shmid, IPC_RMID, NULL);
  return 0;
}