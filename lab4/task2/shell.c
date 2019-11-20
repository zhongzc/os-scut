#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include "utils.h"

#define MAXARGV 16
#define MAXPIPE 8
static char PROMPT[] = "@ ";

struct cmd {
  char type;
  char *argv[MAXARGV];
  char *inRedir;
  char *outRedir;
  char isBackground;
};

struct pipecmd {
  struct cmd cmds[MAXPIPE];
};

// Command Type Bit
#define PIPE_T (1 << 0)
#define INREDIR_T (1 << 1)
#define OUTREDIR_T (1 << 2)

// Read raw command
static char buf[512];
// Parsed command
static struct pipecmd p;

// util function for parsing
char *findFirstNotIn(char *from, char *limit, char *charset);
char *findFirstIn(char *from, char *limit, char *charset);

// Parsing
int parseCmd();
void parseOneCmd(char *begin, char *end, int index);

// Two kinds of commands, need to run at a new process
// runCmd dispatch a command
void runPipe(struct cmd *left, struct cmd *right);
void runCommom(struct cmd *);
void runCmd(struct cmd *p);

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

// Parse a list of pipe. CMD := OneCMD { | OneCMD }
int parseCmd() {
  int len = strlen(buf);
  if (len == 1) {
    return 0;
  }
  buf[--len] = 0;

  char *end = buf + len;
  char *begin = buf, *cur = buf;
  for (int i = 0; i < MAXPIPE && cur < end; i++) {
    while (cur < end && *cur != '|') {
      cur++;
    }
    parseOneCmd(begin, cur, i);

    if (*cur == '|') {
      p.cmds[i].type |= PIPE_T;
    }
    cur++;
    begin = cur;
  }
  return 1;
}

// Parse One Command. OneCMD := ARGV+ [< INFILE] [> OUTFILE] [&]
void parseOneCmd(char *begin, char *end, int index) {
  static char WHITESPACES[] = " \v\f\t\r\n";
  p.cmds[index].type = 0;
  p.cmds[index].isBackground = 0;
  char *cur = begin;
  int flag = 0;
  for (int i = 0; i < MAXARGV && cur < end; i++) {
    cur = findFirstNotIn(cur, end, WHITESPACES);
    begin = cur;
    switch (flag) {
      case 1:
        p.cmds[index].inRedir = begin;
        break;
      case 2:
        p.cmds[index].outRedir = begin;
        break;
      default:
        p.cmds[index].argv[i] = begin;
        break;
    }
    cur = findFirstIn(cur, end, " \v\f\t\r\n");
    *cur++ = 0;
    cur = findFirstNotIn(cur, end, WHITESPACES);
    switch (*cur) {
      case '<':
        flag = 1;
        p.cmds[index].type |= INREDIR_T;
        cur++;
        break;
      case '>':
        flag = 2;
        p.cmds[index].type |= OUTREDIR_T;
        cur++;
        break;
      case '&':
        p.cmds[index].isBackground = 1;
        cur++;
        break;
    }
  }
}

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

void handleRedir(char rdy, int cls, char *file, const char *mode) {
  if (rdy) {
    closeit(cls);
    openit(file, mode);
  }
}
void runCommom(struct cmd *cmd) {
  handleRedir(cmd->type & INREDIR_T, 0, cmd->inRedir, "r");
  handleRedir(cmd->type & OUTREDIR_T, 1, cmd->outRedir, "w+");
  if (cmd->isBackground) {
    int f = forkit();
    if (f == 0) {
      execit(cmd->argv[0], cmd->argv);
    } else {
      waitpidit(f);
      printf("\n[%i] done\n", getpid());
      printf("%s", PROMPT);
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

// util function for parsing
char *findFirstNotIn(char *from, char *limit, char *charset) {
  int len = strlen(charset);
  while (from < limit) {
    int i = 0;
    for (; i < len; i++) {
      if (charset[i] == *from) {
        from++;
        break;
      }
    }
    if (i == len) {
      return from;
    }
  }
  return from;
}
char *findFirstIn(char *from, char *limit, char *charset) {
  while (from < limit) {
    for (int i = 0; i < strlen(charset); i++) {
      if (charset[i] == *from) {
        return from;
      }
    }
    from++;
  }
  return from;
}