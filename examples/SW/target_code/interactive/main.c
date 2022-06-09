#include <stdio.h>
#include <stdlib.h>

int main()
{
    char buf[1024];

    printf("Write something\n");
    while (1)
    {
        fgets(buf, 1024, stdin);
        if (buf[0] == 'q')
        {
            exit(0);
        }
        printf("You wrote: %s", buf);
    }
}
