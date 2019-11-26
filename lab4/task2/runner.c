#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "common.h"
#include "doit.h"

void runPipe(struct cmd *, struct cmd *);
void runCommom(struct cmd *);

// runCmd dispatch a command
void runCmd(struct cmd *p) {
  int f = forkit();
  if (f == 0) {
    if (p->type & PIPE_T) {
      runPipe(p, p + 1);
    } else {
      runCommom(p);
    }
  } else {
    if (p->isBackground) {
      printf("backgound: pid [%i]\n", f);
    } else {
      waitpidit(f);
    }
  }
}

void handleRedir(char rdy, int cls, char *file, int mode, int protect) {
  if (rdy) {
    closeit(cls);
    openit(file, mode, protect);
  }
}
void runCommom(struct cmd *cmd) {
  handleRedir(cmd->type & INREDIR_T, 0, cmd->inRedir, O_CREAT | O_RDONLY, 0644);
  handleRedir(cmd->type & OUTREDIR_T, 1, cmd->outRedir,
              O_CREAT | O_WRONLY | O_TRUNC, 0644);
  if (cmd->isBackground) {
    int f = forkit();
    if (f == 0) {
      execit(cmd->argv[0], cmd->argv);
    } else {
      waitpidit(f);
      printf("\n[%i] done\n", getpid());
      updateCwd();
      printPrompt();
      exit(0);
    }
  } else {
    execit(cmd->argv[0], cmd->argv);
  }
}

void handlePipe(int *p, int isSrc) {
  closeit(p[!isSrc]);
  closeit(isSrc);
  dupit(p[isSrc]);
  closeit(p[isSrc]);
}
void runPipe(struct cmd *left, struct cmd *right) {
  int p[2];
  pipeit(p);
  int f1 = forkit();
  if (f1 == 0) {
    handlePipe(p, 1);
    runCommom(left);
  } else {
    handlePipe(p, 0);
    runCmd(right);

    waitpidit(f1);
    exit(0);
  }
}