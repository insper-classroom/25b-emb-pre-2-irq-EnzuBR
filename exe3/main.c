#include <stdio.h>
#include "hardware/gpio.h"
#include "pico/stdlib.h"

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;

volatile int g_flag_red = 0;
volatile int g_flag_green = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (events & GPIO_IRQ_EDGE_FALL) {
        if (gpio == BTN_PIN_R) {
            g_flag_red = 1;
        } else if (gpio == BTN_PIN_G) {
            g_flag_green = 1;
        }
    }
}

int main() {
    stdio_init_all();

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
    gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);

    while (true) {
        if (g_flag_red) {
            g_flag_red = 0;
            printf("fall red\n");
        }
        if (g_flag_green) {
            g_flag_green = 0;
            printf("fall green\n");
        }
        sleep_ms(1);
    }
}
