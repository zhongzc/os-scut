#include <stdio.h>

int forkit();
int execit(char *path, char **argv);
int closeit(int fd);
FILE *openit(const char *path, const char *mode);
int pipeit(int *p);
int dupit(int d);
int waitpidit(int pid);
int chdirit(const char *dir);
