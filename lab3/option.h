#ifndef FILE_OPTION
#define FILE_OPTION

#include <dirent.h>
#include <sys/stat.h>

struct filter {
  enum { f_empty, f_name, f_ctime, f_mtime } type;
  union {
    const char *name;
    int n;
  };
};

struct option {
  const char *prunePath;
  struct filter filters[6];
};

int match(const char *string, const char *pattern);
int doFilter(struct filter *, struct dirent *, struct stat *);
int doFilters(struct option *, struct dirent *, struct stat *);
void filtersAllTrue(struct option*);

#endif /* !FILE_OPTION */