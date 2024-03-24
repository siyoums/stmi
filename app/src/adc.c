#include "adc.h"
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/rcc.h>

void adc_setup(void) {
  rcc_periph_clock_enable(RCC_ADC1);

  adc_power_off(ADC1);

  adc_disable_scan_mode(ADC1);
  adc_set_continuous_conversion_mode(ADC1);

  adc_set_sample_time(ADC1, ADC_CHANNEL11, ADC_SMPR_SMP_28CYC);

  adc_enable_eoc_interrupt(ADC1);

  adc_power_on(ADC1);

  // wait for adc stargin up (will be replaced with systick delay later)
  for (uint32_t i = 0; i < 800000; i++) {
    __asm__("nop");
  }
}

// uint16_t adc_convert(void) {
//   uint8_t channels[] = {ADC_CHANNEL11};

//   adc_set_regular_sequence(ADC1, sizeof(channels) / sizeof(uint8_t),
//   channels); adc_start_conversion_regular(ADC1); while (!adc_eoc(ADC1))
//     ;
//   adc_read_regular(ADC1);
// }