#ifndef FILE_DOIT
#define FILE_DOIT

#include <stdio.h>

int forkit();
int execit(char *path, char **argv);
int closeit(int fd);
int openit(const char *path, int mode, int protect);
int pipeit(int *p);
int dupit(int d);
int waitpidit(int pid);
int chdirit(const char *dir);

#endif /* !FILE_DOIT */