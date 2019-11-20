#include "common.h"
#include "parser.h"
#include <string.h>

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

void parseOneCmd(char *, char *, int);

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