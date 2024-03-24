#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "adc.h"
#include "core/system.h"
#include "timer.h"
#include <libopencm3/cm3/scb.h>

#include "core/uart.h"

#define BOOTLOADER_SIZE (0x10000U)

#define LED_PORT (GPIOD)
#define LED_PIN (GPIO15)

// uart defines
#define UART_PORT (GPIOA)
#define TX_PIN (GPIO2)
#define RX_PIN (GPIO3)

// adc defines
#define ADC_PIN (GPIO14)

// i2c defines
#define I2C_PORT (GPIOB)
#define SDA_PIN (GPIO11)
#define SCL_PIN (GPIO10)

static void vector_setup(void) { SCB_VTOR = BOOTLOADER_SIZE; }

static void gpio_setup(void) {
  rcc_periph_clock_enable(RCC_GPIOA | RCC_GPIOD | RCC_GPIOB);

  // pwm led phase blink thing
  gpio_mode_setup(LED_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, LED_PIN);
  gpio_set_af(LED_PORT, GPIO_AF2, LED_PIN);

  // uart
  gpio_mode_setup(UART_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, TX_PIN | RX_PIN);
  gpio_set_af(UART_PORT, GPIO_AF7, TX_PIN | RX_PIN);

  // adc
  gpio_mode_setup(LED_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, ADC_PIN);

  // i2c for lcd module
  gpio_mode_setup(I2C_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, SDA_PIN | SCL_PIN);
  gpio_set_af(I2C_PORT, GPIO_AF4, SDA_PIN | SCL_PIN);
}

// lcd init, will be removed in final fw
// void lcd_init(void) {
//   uint32_t start_time = system_get_ticks();

//   while (1) {
//     if (system_get_ticks() - start_time >= 50) {
//     }
//   }
// }

int main(void) {
  vector_setup();
  system_setup();
  gpio_setup();
  timer_setup();
  uart_setup();
  adc_setup();

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