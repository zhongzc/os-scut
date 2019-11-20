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
char buf[512];
// Parsed command
struct pipecmd p;