#include "option.h"
#include "parser.h"
#include "runner.h"

int main(int argc, char const *argv[]) {
  struct option opt;
  const char *path = parseArgs(argc, argv, &opt);
  runFind(path, &opt);
  return 0;
}
