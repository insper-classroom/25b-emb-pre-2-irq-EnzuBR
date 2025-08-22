#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;

volatile int g_flag_fall = 0;
volatile int g_flag_rise = 0;

void btn_callback(uint gpio, uint32_t events) {
  if (events & GPIO_IRQ_EDGE_FALL) g_flag_fall = 1;
  if (events & GPIO_IRQ_EDGE_RISE) g_flag_rise = 1;
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_set_irq_enabled_with_callback(
      BTN_PIN_R, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

  while (true) {
    if (g_flag_fall) {
      g_flag_fall = 0;
      printf("fall \n");
    }
    if (g_flag_rise) {
      g_flag_rise = 0;
      printf("rise \n");
    }
    sleep_ms(1);
  }
}
