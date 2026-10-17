#include <fan.h>
#include <board.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>


static uint32_t pwm_period;

void fan_setup(void) {
    rcc_periph_clock_enable(RCC_TIM3);

    // TIM3 runs at 2 x APB1 = 90MHz
    pwm_period = rcc_apb1_frequency * 2 / FAN_PWM_FREQ;

    timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM3, 0);
    timer_enable_preload(TIM3);
    timer_continuous_mode(TIM3);
    timer_set_period(TIM3, pwm_period - 1);

    timer_set_oc_mode(TIM3, TIM_OC2, TIM_OCM_PWM1);
    timer_enable_oc_preload(TIM3, TIM_OC2);
    timer_set_oc_value(TIM3, TIM_OC2, 0);
    timer_enable_oc_output(TIM3, TIM_OC2);

    timer_enable_counter(TIM3);

    // R30 keeps the gate low until here
    gpio_mode_setup(FAN_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, FAN_PIN);
    gpio_set_af(FAN_PORT, GPIO_AF2, FAN_PIN);
}

void fan_set(uint16_t speed) {
    if (speed > FAN_MAX) speed = FAN_MAX;
    timer_set_oc_value(TIM3, TIM_OC2, (uint32_t)speed * pwm_period / FAN_MAX);
}
