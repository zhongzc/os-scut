#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    pid_t pid = fork();

    if (pid == 0)
    {
        char *argv[] = {"./an_ch2_1b", 0};
        execv("./an_ch2_1b", argv);
    }
    else
    {
        while (1)
        {
            time_t timer = time(NULL);
            struct tm *t = localtime(&timer);
            printf("Those output come from parent, %s", asctime(t));
        }
    }
    return 0;
}
