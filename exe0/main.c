#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;

volatile int btn_flag;

void btn_callback(uint gpio, uint32_t events) {
  if (events & GPIO_IRQ_EDGE_FALL) { // fall edge
    btn_flag = 1;
  }
}

int main() {
  stdio_init_all();
  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);
  gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, &btn_callback);

  int capture_flag = 0;

  while (1) {
    if (btn_flag) {
      capture_flag = 1;
      btn_flag = 0;
    }

    if (capture_flag) {
      printf("btn pressed \n");

      while (!gpio_get(BTN_PIN_R)) {
        sleep_ms(1);
      }

      printf("btn released \n");

      sleep_ms(1);
      capture_flag = 0;
    }
  }
}
