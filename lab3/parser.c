
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "option.h"

void printHelp() {
  printf("Usage: myfind PATH -option [option]... [-print]\n");
  printf("\n");
  printf("Can specify the following options to filter results:\n");
  printf("  -name  FILENAME\tthe file name to be found, can be a\n");
  printf("                 \tregular expression\n\n");
  printf("  -prune DIRNAME \tthe directory name to ignore\n\n");
  printf("  -mtime N       \tN: +n\tfiles modified n days ago\n");
  printf("                 \t   -n\tfiles modified within n days\n\n");
  printf("  -ctime N       \tN: +n\tfiles created n days ago\n");
  printf("                 \t   -n\tfiles created within n days\n");
  printf("\n");
  printf("Other option:\n");
  printf("  -print         \tprint the output to standard output\n");
  exit(-1);
}

void addFilter(struct option *opt, struct filter filter) {
  struct filter *f = opt->filters;
  while (f->type != f_empty) f++;
  *f = filter;
}

int sameStr(const char *left, const char *right) {
  return strlen(left) == strlen(right) && strcmp(left, right) == 0;
}

void parseName(int *idx, char const *argv[], struct option *opt) {
  if (argv[*idx] == NULL) {
    printHelp();
  }
  struct filter f;
  f.type = f_name;
  f.name = argv[*idx];
  addFilter(opt, f);
  (*idx)++;
}
void parsePrune(int *idx, char const *argv[], struct option *opt) {
  if (argv[*idx] == NULL) {
    printHelp();
  }
  opt->prunePath = argv[*idx];
  (*idx)++;
}

int parseAddSubTime(int *idx, char const *argv[], int *res) {
  if (argv[*idx][0] == '-' && '0' <= argv[*idx][1] && argv[*idx][1] <= '9') {
    *res = atoi(argv[*idx]);
    return 1;
  } else if (argv[*idx][0] == '+' && '0' <= argv[*idx][1] &&
             argv[*idx][1] <= '9') {
    *res = atoi(argv[*idx]);
    return 1;
  } else {
    return 0;
  }
}

void parseMtime(int *idx, char const *argv[], struct option *opt) {
  if (argv[*idx] == NULL) {
    printHelp();
  }
  int res;
  if (parseAddSubTime(idx, argv, &res)) {
    struct filter f;
    f.type = f_mtime;
    f.n = res;
    addFilter(opt, f);
    (*idx)++;
    if (argv[*idx] != NULL && parseAddSubTime(idx, argv, &res)) {
      struct filter f;
      f.type = f_mtime;
      f.n = res;
      addFilter(opt, f);
      (*idx)++;
    }
  } else {
    printHelp();
  }
}
void parseCtime(int *idx, char const *argv[], struct option *opt) {
  if (argv[*idx] == NULL) {
    printHelp();
  }
  int res;
  if (parseAddSubTime(idx, argv, &res)) {
    struct filter f;
    f.type = f_ctime;
    f.n = res;
    addFilter(opt, f);
    (*idx)++;
    if (argv[*idx] != NULL && parseAddSubTime(idx, argv, &res)) {
      struct filter f;
      f.type = f_ctime;
      f.n = res;
      addFilter(opt, f);
      (*idx)++;
    }
  } else {
    printHelp();
  }
}

const char *parseArgs(int argc, char const *argv[], struct option *opt) {
  opt->prunePath = NULL;
  for (int i = 0; i < 6; i++) {
    opt->filters[i].type = f_empty;
  }

  if (argc < 4) {
    printHelp();
  }

  if (!sameStr(argv[2], "-option")) {
    printHelp();
  }

  int idx = 3;
  while (idx < argc) {
    if (sameStr(argv[idx], "-name")) {
      idx++;
      parseName(&idx, argv, opt);
    } else if (sameStr(argv[idx], "-prune")) {
      idx++;
      parsePrune(&idx, argv, opt);
    } else if (sameStr(argv[idx], "-mtime")) {
      idx++;
      parseMtime(&idx, argv, opt);
    } else if (sameStr(argv[idx], "-ctime")) {
      idx++;
      parseCtime(&idx, argv, opt);
    } else if (sameStr(argv[idx], "-print")) {
      idx++;
    } else {
      printHelp();
    }
  }

  return argv[1];
}