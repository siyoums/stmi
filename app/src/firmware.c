#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "core/system.h"
#include "timer.h"
#include <libopencm3/cm3/scb.h>

#include "core/uart.h"

#define BOOTLOADER_SIZE (0x10000U)

#define LED_PORT (GPIOD)
#define LED_PIN (GPIO15)

#define UART_PORT (GPIOA)
#define TX_PIN (GPIO2)
#define RX_PIN (GPIO3)

static void vector_setup(void) { SCB_VTOR = BOOTLOADER_SIZE; }

static void gpio_setup(void) {
  rcc_periph_clock_enable(RCC_GPIOD);

  // pwm led phase blink thing
  gpio_mode_setup(LED_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, LED_PIN);
  gpio_set_af(LED_PORT, GPIO_AF2, LED_PIN);

  gpio_mode_setup(UART_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, TX_PIN | RX_PIN);
}

int main(void) {
  vector_setup();
  system_setup();
  gpio_setup();
  timer_setup();

  float duty_cycle = 0.0f;

  uint32_t start_time = system_get_ticks();

  while (1) {
    // pwm led phase
    if (system_get_ticks() - start_time >= 10) {
      duty_cycle += 1.0f;
      if (duty_cycle > 100.0f) {
        duty_cycle = 0;
      }
      timer_set_pwm_duty_cycle(duty_cycle);
      start_time = system_get_ticks();
    }

    if (uart_data_available()) {
      uint8_t data = uart_read_byte();
      uart_write_byte(data + 1);
    }
    // Do useful work
  }
}