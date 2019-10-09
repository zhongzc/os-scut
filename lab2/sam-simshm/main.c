#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void exec_writer() {
  if (fork() == 0) {
    const char *writer = "./writer";
    execl(writer, writer, 0);
  }
  wait(NULL);
}

void exec_reader() {
  if (fork() == 0) {
    const char *reader = "./reader";
    execl(reader, reader, 0);
  }
  wait(NULL);
}

int main(void) {
  key_t semkey = ftok(".", 42);
  int shmid = shmget(semkey, 1024 * sizeof(char), IPC_CREAT | 0660);

  exec_writer();
  exec_reader();

  shmctl(shmid, IPC_RMID, NULL);
  return 0;
}