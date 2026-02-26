#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"

// ---------------- LED callbacks ----------------

static void led_on_callback(const char* args)
{
    (void)args;
    led_task_state_set(LED_STATE_ON);
}

static void led_off_callback(const char* args)
{
    (void)args;
    led_task_state_set(LED_STATE_OFF);
}

static void led_blink_callback(const char* args)
{
    (void)args;
    led_task_state_set(LED_STATE_BLINK);
}

static void led_blink_set_period_ms_callback(const char* args)
{
    uint32_t period_ms = 0;

    if (args != NULL)
    {
        sscanf(args, "%u", &period_ms);
    }

    if (period_ms == 0)
    {
        printf("Error: period_ms must be > 0\r\n");
        printf("Usage: led_blink_set_period_ms <period_ms>\r\n");
        return;
    }

    led_task_set_blink_period_ms(period_ms);
    printf("OK: blink period set to %u ms\r\n", period_ms);
}

// ---------------- mem / wmem ----------------

static int parse_hex_u32(const char *s, uint32_t *out)
{
    if (!s) return 0;

    while (*s && isspace((unsigned char)*s)) s++;

    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) s += 2;

    char *end = NULL;
    unsigned long v = strtoul(s, &end, 16);
    if (end == s) return 0;

    *out = (uint32_t)v;
    return 1;
}

static void mem_callback(const char *args)
{
    uint32_t addr = 0;

    if (!parse_hex_u32(args, &addr))
    {
        printf("Error: usage: mem <hex_addr>\r\n");
        return;
    }

    volatile uint32_t *p = (volatile uint32_t *)addr;
    uint32_t value = *p;

    printf("OK: mem[0x%08lx] = 0x%08lx (%lu)\r\n",
           (unsigned long)addr,
           (unsigned long)value,
           (unsigned long)value);
}

static void wmem_callback(const char *args)
{
    if (!args)
    {
        printf("Error: usage: wmem <hex_addr> <hex_value>\r\n");
        return;
    }

    char buf[128];
    size_t n = 0;
    while (args[n] && n < sizeof(buf) - 1)
    {
        buf[n] = args[n];
        n++;
    }
    buf[n] = '\0';

    char *a = buf;
    while (*a && isspace((unsigned char)*a)) a++;

    char *space = a;
    while (*space && !isspace((unsigned char)*space)) space++;

    if (!*space)
    {
        printf("Error: usage: wmem <hex_addr> <hex_value>\r\n");
        return;
    }

    *space++ = '\0';

    while (*space && isspace((unsigned char)*space)) space++;

    uint32_t addr = 0;
    uint32_t value = 0;

    if (!parse_hex_u32(a, &addr) ||
        !parse_hex_u32(space, &value))
    {
        printf("Error: usage: wmem <hex_addr> <hex_value>\r\n");
        return;
    }

    volatile uint32_t *p = (volatile uint32_t *)addr;
    *p = value;

    printf("OK: wmem[0x%08lx] <- 0x%08lx\r\n",
           (unsigned long)addr,
           (unsigned long)value);
}

// ---------------- device api table ----------------

static api_t device_api[] =
{
    { "on",    led_on_callback,    "Turn LED ON" },
    { "off",   led_off_callback,   "Turn LED OFF" },
    { "blink", led_blink_callback, "Blink LED (non-blocking)" },

    { "led_blink_set_period_ms",
      led_blink_set_period_ms_callback,
      "Set blink period in ms. Usage: led_blink_set_period_ms 1000" },

    { "mem",  mem_callback,  "Read 32-bit word. Usage: mem d0000014" },
    { "wmem", wmem_callback, "Write 32-bit word. Usage: wmem d0000014 02000000" },

    { NULL, NULL, NULL }
};

// ---------------- main ----------------

int main()
{
    stdio_init_all();
    sleep_ms(1500);

    led_task_init();
    protocol_task_init(device_api);

    char cmd[256];
    size_t pos = 0;

    while (true)
    {
        led_task_handle();

        int ch = getchar_timeout_us(0);

        if (ch != PICO_ERROR_TIMEOUT)
        {if (ch == '\r' || ch == '\n')
            {
                if (pos > 0)
                {
                    cmd[pos] = '\0';
                    protocol_task_handle(cmd);
                    pos = 0;
                }
            }
            else
            {
                if (pos < sizeof(cmd) - 1)
                {
                    cmd[pos++] = (char)ch;
                }
                else
                {
                    pos = 0;
                }
            }
        }

        sleep_ms(5);
    }
}