#include <motors.h>
#include <board.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>


static uint32_t pwm_period;

// With EN high each MP6551 output follows its IN pin. One input stays high and
// the other is pulled low for the duty part of the period; the rest of the
// period both are high, so the motor brakes through the high side (slow decay).
// A compare value of pwm_period keeps the output high for the whole period.

void motors_setup(void) {
    rcc_periph_clock_enable(RCC_TIM1);

    gpio_mode_setup(MOTOR_EN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, MOTOR_EN_PIN);
    gpio_clear(MOTOR_EN_PORT, MOTOR_EN_PIN);

    // nFAULT may be open drain
    gpio_mode_setup(MOTOR_FAULT_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP,
                    MA_FAULT_PIN | MB_FAULT_PIN);

    // TIM1 runs at 2 x APB2 = 180MHz
    pwm_period = rcc_apb2_frequency * 2 / MOTOR_PWM_FREQ;

    timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM1, 0);
    timer_set_repetition_counter(TIM1, 0);
    timer_enable_preload(TIM1);
    timer_continuous_mode(TIM1);
    timer_set_period(TIM1, pwm_period - 1);

    const enum tim_oc_id channels[] = {TIM_OC1, TIM_OC2, TIM_OC3, TIM_OC4};
    for (unsigned i = 0; i < sizeof(channels) / sizeof(channels[0]); i++) {
        timer_set_oc_mode(TIM1, channels[i], TIM_OCM_PWM1);
        timer_enable_oc_preload(TIM1, channels[i]);
        timer_set_oc_value(TIM1, channels[i], pwm_period);
        timer_enable_oc_output(TIM1, channels[i]);
    }

    timer_enable_break_main_output(TIM1);  // needed on advanced timers
    timer_enable_counter(TIM1);

    gpio_mode_setup(MOTOR_PWM_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, MOTOR_PWM_PINS);
    gpio_set_af(MOTOR_PWM_PORT, GPIO_AF1, MOTOR_PWM_PINS);
}

void motors_enable(bool enable) {
    if (enable) gpio_set(MOTOR_EN_PORT, MOTOR_EN_PIN);
    else gpio_clear(MOTOR_EN_PORT, MOTOR_EN_PIN);
}

static void motor_set(enum tim_oc_id in1, enum tim_oc_id in2, int16_t speed) {
    if (speed > MOTOR_MAX) speed = MOTOR_MAX;
    if (speed < -MOTOR_MAX) speed = -MOTOR_MAX;

    uint32_t duty = (uint32_t)(speed < 0 ? -speed : speed) * pwm_period / MOTOR_MAX;
    if (speed >= 0) {
        // OUT1 high, OUT2 low during duty
        timer_set_oc_value(TIM1, in1, pwm_period);
        timer_set_oc_value(TIM1, in2, pwm_period - duty);
    } else {
        timer_set_oc_value(TIM1, in1, pwm_period - duty);
        timer_set_oc_value(TIM1, in2, pwm_period);
    }
}

void motors_set(int16_t speed_a, int16_t speed_b) {
    motor_set(TIM_OC1, TIM_OC2, speed_a);
    motor_set(TIM_OC3, TIM_OC4, speed_b);
}

bool motor_fault_a(void) {
    return !gpio_get(MOTOR_FAULT_PORT, MA_FAULT_PIN);
}

bool motor_fault_b(void) {
    return !gpio_get(MOTOR_FAULT_PORT, MB_FAULT_PIN);
}
