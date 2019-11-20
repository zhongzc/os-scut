#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "doit.h"

// Checked version System call
int forkit() {
  int f = fork();
  if (f < 0) {
    fprintf(stderr, "sh: can not fork\n");
    exit(-1);
  }
  return f;
}
int execit(char *path, char **argv) {
  int e = execvp(path, argv);
  if (e < 0) {
    fprintf(stderr, "sh: can not exec %s\n", path);
    exit(-1);
  }
  return e;
}
int closeit(int fd) {
  int c = close(fd);
  if (c < 0) {
    fprintf(stderr, "sh: can not close %d\n", fd);
    exit(-1);
  }
  return c;
}
FILE *openit(const char *path, const char *mode) {
  FILE *f = fopen(path, mode);
  if (f == NULL) {
    fprintf(stderr, "sh: can not open %s\n", path);
    exit(-1);
  }
  return f;
}
int pipeit(int *p) {
  int r = pipe(p);
  if (r < 0) {
    fprintf(stderr, "sh: can not pipe\n");
    exit(-1);
  }
  return r;
}
int dupit(int d) {
  int r = dup(d);
  if (r < 0) {
    fprintf(stderr, "sh: can not dup\n");
    exit(-1);
  }
  return r;
}
int waitpidit(int pid) {
  int r = waitpid(pid, NULL, 0);
  if (r < 0) {
    fprintf(stderr, "sh: can not wait\n");
    exit(-1);
  }
  return r;
}
int chdirit(const char *dir) {
  int r = chdir(dir);
  if (r < 0) {
    fprintf(stderr, "sh: can not change working directory\n");
  }
  return r;
}
