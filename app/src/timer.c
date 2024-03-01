#include "timer.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

// 168,000,000
#define PRESCALER (168)
#define AUTO_RELOAD (1000)

void timer_setup(void) {
  // Enable clock for the tim2 peripheral
  rcc_periph_clock_enable(RCC_TIM4);

  // High level timer setup
  timer_set_mode(TIM4, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

  // Pwm mode on
  timer_set_oc_mode(TIM4, TIM_OC4, TIM_OCM_PWM1);

  // Enable pwm output
  timer_enable_counter(TIM4);
  timer_enable_oc_output(TIM4, TIM_OC4);

  // Tim2 freq and resolution for the pwm signal
  timer_set_prescaler(TIM4, PRESCALER - 1);
  timer_set_period(TIM4, AUTO_RELOAD - 1);
}

void timer_set_pwm_duty_cycle(float duty_cycle) {
  const float ccr = (float)AUTO_RELOAD * (duty_cycle / 100.0f);
  timer_set_oc_value(TIM4, TIM_OC4, (uint32_t)ccr);
}
