#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

static const int LED_PIN = 4;
static const int BTN_PIN = 28;

volatile int g_flag_press = 0;

static void btn_callback(uint gpio, uint32_t events) {
    if (events & GPIO_IRQ_EDGE_FALL) {
        g_flag_press = 1;
    }
}

int main() {
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);

    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_pull_up(BTN_PIN);

    gpio_set_irq_enabled_with_callback(
        BTN_PIN, GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    int led_state = 0;

    while (true) {
        if (g_flag_press) {
            g_flag_press = 0;
            led_state = !led_state;
            gpio_put(LED_PIN, led_state);
        }
        sleep_ms(1);
    }
}
