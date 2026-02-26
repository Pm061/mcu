#include <stdio.h>
#include "pico/stdlib.h"

int main()
{
    stdio_init_all();

    while (true)
    {
        int c = getchar_timeout_us(0);

        if (c != PICO_ERROR_TIMEOUT)
        {
            printf("received char: %c [ ASCII code: %d ]\n", c, c);
        }

        sleep_ms(10);
    }
}