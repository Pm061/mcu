#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    stdio_init_all();

    int heap_variable = 5555;
    const int constant_variable = 42;
    const char *constant_string = "RP2040 Pico";

    while (true) {
        printf("Hello World!\n");
        printf("Device: %s\n", constant_string);
        printf("constant variable | value = %d\n", constant_variable);
        printf("heap variable | value = %d\n", heap_variable);
        printf("-----------------------------\n");
        sleep_ms(1000);
    }
}