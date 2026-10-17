#include <fan.h>
#include <board.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>


static uint32_t pwm_period;

void fan_setup(void) {
    rcc_periph_clock_enable(RCC_TIM1);

    // TIM1 runs at 2 x APB2 = 180MHz
    pwm_period = rcc_apb2_frequency * 2 / FAN_PWM_FREQ;

    timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM1, 0);
    timer_set_repetition_counter(TIM1, 0);
    timer_enable_preload(TIM1);
    timer_continuous_mode(TIM1);
    timer_set_period(TIM1, pwm_period - 1);

    timer_set_oc_mode(TIM1, TIM_OC4, TIM_OCM_PWM1);
    timer_enable_oc_preload(TIM1, TIM_OC4);
    timer_set_oc_value(TIM1, TIM_OC4, 0);
    timer_enable_oc_output(TIM1, TIM_OC4);

    timer_enable_break_main_output(TIM1);  // needed on advanced timers
    timer_enable_counter(TIM1);

    // R30 keeps the gate low until here
    gpio_mode_setup(FAN_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, FAN_PIN);
    gpio_set_af(FAN_PORT, GPIO_AF1, FAN_PIN);
}

void fan_set(uint16_t speed) {
    if (speed > FAN_MAX) speed = FAN_MAX;
    timer_set_oc_value(TIM1, TIM_OC4, (uint32_t)speed * pwm_period / FAN_MAX);
}
