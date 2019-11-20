#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "doit.h"
#include "parser.h"
#include "runner.h"

int main(int argc, char *argv[]) {
  printf("%s", PROMPT);
  for (fgets(buf, sizeof(buf), stdin); buf[0] != 0;
       fgets(buf, sizeof(buf), stdin)) {
    if (parseCmd()) {
      if (strcmp(p.cmds->argv[0], "cd") == 0) {
        chdirit(p.cmds->argv[1]);
      } else if (strcmp(p.cmds->argv[0], "exit") == 0) {
        exit(0);
      } else {
        runCmd(p.cmds);
      }
    }

    printf("%s", PROMPT);
    memset(buf, 0, sizeof(buf));
    memset(&p, 0, sizeof(p));
  }

  exit(0);
}
