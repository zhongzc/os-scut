#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    pid_t pid = fork();

    if (pid == 0)
    {
        char *const* as = {NULL};
        execv("./an_ch2_1b", as);
    }
    else
    {
        while (1)
        {
            time_t timer = time(NULL);
            printf("Those output come from parent, %ld\n", timer);
        }
    }
    return 0;
}
