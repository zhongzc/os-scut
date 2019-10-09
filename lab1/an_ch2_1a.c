#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int main() {
  pid_t pid = fork();

  if (pid == 0) {
    const char* CHILD = "./an_ch2_1b";
    execl(CHILD, CHILD, 0);
  } else {
    while (1) {
      time_t timer = time(NULL);
      struct tm *t = localtime(&timer);
      printf("Those output come from parent, %s", asctime(t));
    }
  }
  return 0;
}
