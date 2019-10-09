#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include "common.h"

int main(int argc, char const *argv[]) {
  key_t semkey = ftok(FTOK_PATH, FTOK_ID);
  int shmid = shmget(semkey, 1024 * sizeof(char), 0660);
  char *str = shmat(shmid, NULL, 0);

  const char *s = "hello world";
  memcpy(str, s, strlen(s) + 1);
  printf("%d: Data written in memory: %s\n", getpid(), str);

  shmdt(str);
  return 0;
}
