#include "led-task.h"

#include "pico/stdlib.h"
#include "hardware/gpio.h"

static const uint LED_PIN = 25;

/* ВАЖНО: без const */
static uint64_t LED_BLINK_PERIOD_US = 500000;

static led_state_t led_state = LED_STATE_OFF;
static uint64_t led_ts = 0;
static bool led_level = false;

void led_task_state_set(led_state_t state)
{
    led_state = state;
}

void led_task_init(void)
{
    led_state = LED_STATE_OFF;
    led_ts = 0;
    led_level = false;

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);
}

void led_task_handle(void)
{
    switch (led_state)
    {
        case LED_STATE_OFF:
            gpio_put(LED_PIN, 0);
            led_level = false;
            break;

        case LED_STATE_ON:
            gpio_put(LED_PIN, 1);
            led_level = true;
            break;

        case LED_STATE_BLINK:
        {
            uint64_t now = time_us_64();
            if (now >= led_ts)
            {
                led_ts = now + (LED_BLINK_PERIOD_US / 2);
                led_level = !led_level;
                gpio_put(LED_PIN, led_level ? 1 : 0);
            }
            break;
        }

        default:
            break;
    }
}

/* Новая функция */
void led_task_set_blink_period_ms(uint32_t period_ms)
{
    if (period_ms == 0)
        return;

    LED_BLINK_PERIOD_US = (uint64_t)period_ms * 1000u;
}