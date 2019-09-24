#include <stdio.h>
#include <time.h>

int main(int argc, char const *argv[])
{

    while (1)
    {
        time_t timer = time(NULL);
        printf("Those output come from child, %ld\n", timer);
    }
    return 0;
}
