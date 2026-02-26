#ifndef LED_TASK_H
#define LED_TASK_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    LED_STATE_OFF = 0,
    LED_STATE_ON,
    LED_STATE_BLINK
} led_state_t;

void led_task_init(void);
void led_task_handle(void);
void led_task_state_set(led_state_t state);

/* Новая функция */
void led_task_set_blink_period_ms(uint32_t period_ms);

#endif