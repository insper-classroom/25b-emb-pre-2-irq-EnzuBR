#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

static const int LED_R = 4;
static const int LED_G = 6;
static const int BTN_R = 28;
static const int BTN_G = 26;

volatile int flag_red = 0;
volatile int flag_green = 0;

static void btn_callback(uint gpio, uint32_t events) {
    if ((gpio == BTN_R) && (events & GPIO_IRQ_EDGE_FALL)) flag_red = 1;
    if ((gpio == BTN_G) && (events & GPIO_IRQ_EDGE_RISE)) flag_green = 1;
}

int main() {
    stdio_init_all();

    gpio_init(LED_R); gpio_set_dir(LED_R, GPIO_OUT); gpio_put(LED_R, 0);
    gpio_init(LED_G); gpio_set_dir(LED_G, GPIO_OUT); gpio_put(LED_G, 0);

    gpio_init(BTN_R); gpio_set_dir(BTN_R, GPIO_IN); gpio_pull_up(BTN_R);
    gpio_init(BTN_G); gpio_set_dir(BTN_G, GPIO_IN); gpio_pull_up(BTN_G);

    gpio_set_irq_enabled_with_callback(BTN_R, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
    gpio_set_irq_enabled(BTN_G, GPIO_IRQ_EDGE_RISE, true);

    int led_r_state = 0;
    int led_g_state = 0;

    while (true) {
        if (flag_red) {
            flag_red = 0;
            led_r_state = !led_r_state;
            gpio_put(LED_R, led_r_state);
        }
        if (flag_green) {
            flag_green = 0;
            led_g_state = !led_g_state;
            gpio_put(LED_G, led_g_state);
        }
        sleep_ms(1);
    }
}
