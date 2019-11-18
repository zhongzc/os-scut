#include <pthread.h>
#include <stdio.h>

void *subOne(void *ptr);

int shared_var = 0;

int main() {
  pthread_t thread;
  pthread_create(&thread, NULL, subOne, NULL);

  while (1) {
    printf("In main thread: %d\n", shared_var);
    shared_var++;
  }

  pthread_join(thread, NULL);
  return 0;
}

void *subOne(void *ptr) {
  while (1) {
    printf("In child thread: %d\n", shared_var);
    shared_var--;
  }
}