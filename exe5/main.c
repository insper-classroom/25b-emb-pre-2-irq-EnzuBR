#include <stdio.h>
#include "hardware/gpio.h"
#include "pico/stdlib.h"

static const int BTN_PIN = 28;
static const uint32_t LONG_PRESS_US = 800000;

volatile uint32_t g_press_us = 0;
volatile uint32_t g_release_us = 0;
volatile int g_event_ready = 0;

static void btn_callback(uint gpio, uint32_t events) {
    if (events & GPIO_IRQ_EDGE_FALL) {
        g_press_us = time_us_32();
    }
    if (events & GPIO_IRQ_EDGE_RISE) {
        g_release_us = time_us_32();
        g_event_ready = 1;
    }
}

int main() {
    stdio_init_all();

    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_pull_up(BTN_PIN);

    gpio_set_irq_enabled_with_callback(
        BTN_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &btn_callback);

    while (true) {
        if (g_event_ready) {
            g_event_ready = 0;
            uint32_t dt = g_release_us - g_press_us;
            if (dt >= LONG_PRESS_US) {
                printf("Aperto longo!\n");
            } else {
                printf("Aperto curto!\n");
            }
        }
        sleep_ms(1);
    }
}
