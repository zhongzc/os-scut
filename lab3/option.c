#include "option.h"
#include <regex.h>
#include <stdlib.h>
#include <time.h>

int match(const char *string, const char *pattern) {
  int status;
  regex_t re;
  if (regcomp(&re, pattern, REG_EXTENDED) != 0) {
    return 0;
  }
  status = regexec(&re, string, (size_t)0, NULL, 0);
  regfree(&re);
  if (status != 0) {
    return 0;
  }

  return 1;
}

int doFilter(struct filter *filter, struct dirent *dirent, struct stat *stat) {
  time_t now;
  time(&now);
  switch (filter->type) {
    case f_name:
      return match(dirent->d_name, filter->name);
    case f_ctime:
      if (filter->n > 0) {
        return difftime(now, stat->st_ctime) > filter->n * 86400;
      } else if (filter->n < 0) {
        return difftime(now, stat->st_ctime) < -filter->n * 86400;
      }
      return 0;
    case f_mtime:
      if (filter->n > 0) {
        return difftime(now, stat->st_mtime) > filter->n * 86400;
      } else if (filter->n < 0) {
        return difftime(now, stat->st_mtime) < -filter->n * 86400;
      }
      return 0;
    default:
      return 1;
  }
}

int doFilters(struct option *option, struct dirent *dirent, struct stat *stat) {
  for (int i = 0; i < 6; i++) {
    if (!doFilter(&option->filters[i], dirent, stat)) {
      return 0;
    }
  }
  return 1;
}

void filtersAllTrue(struct option *opt) {
  for (int i = 0; i < 6; i++) {
    opt->filters[i].type = f_empty;
  }
}