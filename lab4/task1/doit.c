#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

long getMill(struct timeval time) {
  return time.tv_sec * 1000 + time.tv_usec / 1000;
}

void printCommand(char *argv[]) {
  printf("\tCommand being timed          = \"");
  while (*argv != NULL) {
    printf("%s ", *argv);
    argv++;
  }

  printf("\b\"\n");
}

int main(int argc, char *argv[]) {
  if (argv[1] == NULL) {
    printf("Usage: doit command [arg...]\n");
    exit(-1);
  }

  struct timeval t1;
  gettimeofday(&t1, NULL);

  pid_t pid = fork();
  if (pid == 0) {
    int f = execvp(argv[1], &argv[1]);
    if (f < 0) {
      fprintf(stderr, "exec fail\n");
      exit(-1);
    }
  }
  waitpid(pid, NULL, 0);
  struct timeval t2;
  gettimeofday(&t2, NULL);

  struct rusage ru;
  getrusage(RUSAGE_CHILDREN, &ru);

  long cpuTime = getMill(ru.ru_stime) + getMill(ru.ru_utime);
  long prcTime = getMill(t2) - getMill(t1);
  long pFaults = ru.ru_majflt + ru.ru_minflt;
  printCommand(&argv[1]);
  printf("\tCPU Time                     = %ld ms\n", cpuTime);
  printf("\tProcess Time                 = %ld ms\n", prcTime);
  printf("\tInvoluntary Context Switches = %ld \n", ru.ru_nivcsw);
  printf("\tVoluntary Context Switches   = %ld \n", ru.ru_nvcsw);
  printf("\tPage Faults                  = %ld \n", pFaults);

  return 0;
}
