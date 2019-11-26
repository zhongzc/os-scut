#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "option.h"
#include "runner.h"
#include "parser.h"

void runFind(const char *path, struct option *opt) {
  DIR *dir = opendir(path);
  if (dir == NULL) {
    fprintf(stderr, "can not open %s\n", path);
    return;
  }
  struct dirent *dirent;
  while ((dirent = readdir(dir)) != NULL) {
    char *newPath = malloc(1024);
    strcpy(newPath, path);
    strcat(newPath, "/");
    strcat(newPath, dirent->d_name);
    struct stat s;
    int r = stat(newPath, &s);
    if (r < 0) {
      fprintf(stderr, "can not stat %s\n", newPath);
      continue;
    }
    if (dirent->d_type & DT_DIR) {
      if (sameStr(dirent->d_name, "..") || sameStr(dirent->d_name, ".")) {
        continue;
      }

      struct option *newOpt = NULL;
      if (doFilters(opt, dirent, &s)) {
        printf("%s\n", newPath);
        newOpt = malloc(sizeof(struct option));
        memcpy(newOpt, opt, sizeof(struct option));
        filtersAllTrue(newOpt);
      }
      if (opt->prunePath == NULL || !sameStr(dirent->d_name, opt->prunePath)) {
        runFind(newPath, newOpt ? newOpt : opt);
      }

    } else if (dirent->d_type & DT_REG) {
      if (doFilters(opt, dirent, &s)) {
        printf("%s\n", newPath);
      }
    }
  }
  closedir(dir);
}