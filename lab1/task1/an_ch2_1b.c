#include <stdio.h>
#include <time.h>

int main()
{
    while (1)
    {
        time_t timer = time(NULL);
        struct tm *t = localtime(&timer);
        printf("Those output come from child,  %s", asctime(t));
    }
    return 0;
}
