#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"

static const uint LED_PIN = 25;

static const char *DEVICE_NAME = "my-pico-device";
static const char *FW_VERSION  = "v0.0.1";

int main() {
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);

    printf("Ready. Commands: e=enable led, d=disable led, v=version\n");

    while (true) {
        int ch = getchar_timeout_us(0);   // не блокирует
        if (ch != PICO_ERROR_TIMEOUT) {
            printf("received char: %c [ ASCII code: %d ]\n", (char)ch, ch);

            if (ch == 'e') {
                gpio_put(LED_PIN, 1);
                printf("led enable done\n");
            } else if (ch == 'd') {
                gpio_put(LED_PIN, 0);
                printf("led disable done\n");
            } else if (ch == 'v') {
                printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, FW_VERSION);
            }
        }

        sleep_ms(10);
    }
}