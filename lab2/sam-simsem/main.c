#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SEM_NAME "/semaphore_example"
#define INITIAL_VALUE 1

#define CHILD_PROGRAM "./sem_chld"

int main(void) {
  /* We initialize the semaphore counter to 1 (INITIAL_VALUE) */
  sem_t *semaphore = sem_open(SEM_NAME, O_CREAT, 0644, INITIAL_VALUE);

  /* Close the semaphore as we won't be using it in the parent process */
  sem_close(semaphore);

  pid_t pids[2];

  for (size_t i = 0; i < sizeof(pids) / sizeof(pids[0]); i++) {
    pids[i] = fork();

    if (pids[i] == 0) {
      execl(CHILD_PROGRAM, CHILD_PROGRAM, NULL);
    }
  }

  for (size_t i = 0; i < sizeof(pids) / sizeof(pids[0]); i++) {
    waitpid(pids[i], NULL, 0);
  }

  sem_unlink(SEM_NAME);

  return 0;
}