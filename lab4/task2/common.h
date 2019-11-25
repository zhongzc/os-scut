#ifndef FILE_COMMON
#define FILE_COMMON

#include <libgen.h>
#include <stdio.h>
#include <unistd.h>

#define MAXARGV 16
#define MAXPIPE 8

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
char buf[512];
// Parsed command
struct pipecmd p;

static char PROMPT[] = "@ ";
static char cwd[256];
static void updateCwd() {
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    char *b = basename(cwd);
    memmove(cwd, b, strlen(b) + 1);
  }
}
static void printPrompt() { printf("%s %s", cwd, PROMPT); }

#endif /* !FILE_COMMON */