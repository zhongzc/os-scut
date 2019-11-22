#ifndef FILE_PARSER
#define FILE_PARSER

#include <string.h>
#include "option.h"

int sameStr(const char *left, const char *right);
const char *parseArgs(int argc, char const *argv[], struct option *opt);

#endif /* !FILE_PARSER */