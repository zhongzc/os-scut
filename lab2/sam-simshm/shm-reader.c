#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
  key_t semkey = ftok(".", 42);
  int shmid = shmget(semkey, 1024 * sizeof(char), 0660);
  char *str = shmat(shmid, NULL, 0);

  printf("%d: Data read from memory: %s\n", getpid(), str);

  shmdt(str);
  return 0;
}
