#include <motors.h>
#include <board.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>


static uint32_t pwm_period;

void motors_setup(void) {
    rcc_periph_clock_enable(RCC_TIM1);

    gpio_mode_setup(MOTOR_PWM_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, MOTOR_PWM_PINS);
    gpio_set_af(MOTOR_PWM_PORT, GPIO_AF1, MOTOR_PWM_PINS);

    gpio_mode_setup(MOTOR_DIR_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                    MA_IN1_PIN | MA_IN2_PIN | MB_IN1_PIN | MB_IN2_PIN);
    gpio_clear(MOTOR_DIR_PORT, MA_IN1_PIN | MA_IN2_PIN | MB_IN1_PIN | MB_IN2_PIN);

    gpio_mode_setup(MOTOR_STBY_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, MOTOR_STBY_PIN);
    gpio_clear(MOTOR_STBY_PORT, MOTOR_STBY_PIN);

    // TIM1 runs at 2 x APB2 = 180MHz
    pwm_period = rcc_apb2_frequency * 2 / MOTOR_PWM_FREQ;

    timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM1, 0);
    timer_set_repetition_counter(TIM1, 0);
    timer_enable_preload(TIM1);
    timer_continuous_mode(TIM1);
    timer_set_period(TIM1, pwm_period - 1);

    timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);
    timer_set_oc_mode(TIM1, TIM_OC2, TIM_OCM_PWM1);
    timer_enable_oc_preload(TIM1, TIM_OC1);
    timer_enable_oc_preload(TIM1, TIM_OC2);
    timer_set_oc_value(TIM1, TIM_OC1, 0);
    timer_set_oc_value(TIM1, TIM_OC2, 0);
    timer_enable_oc_output(TIM1, TIM_OC1);
    timer_enable_oc_output(TIM1, TIM_OC2);

    timer_enable_break_main_output(TIM1);  // needed on advanced timers
    timer_enable_counter(TIM1);
}

void motors_enable(bool enable) {
    if (enable) gpio_set(MOTOR_STBY_PORT, MOTOR_STBY_PIN);
    else gpio_clear(MOTOR_STBY_PORT, MOTOR_STBY_PIN);
}

static void motor_set(enum tim_oc_id oc, uint16_t in1, uint16_t in2, int16_t speed) {
    if (speed > MOTOR_MAX) speed = MOTOR_MAX;
    if (speed < -MOTOR_MAX) speed = -MOTOR_MAX;

    if (speed > 0) {
        gpio_set(MOTOR_DIR_PORT, in1);
        gpio_clear(MOTOR_DIR_PORT, in2);
    } else if (speed < 0) {
        gpio_clear(MOTOR_DIR_PORT, in1);
        gpio_set(MOTOR_DIR_PORT, in2);
        speed = -speed;
    } else {
        // Short brake
        gpio_set(MOTOR_DIR_PORT, in1 | in2);
    }
    timer_set_oc_value(TIM1, oc, (uint32_t)speed * pwm_period / MOTOR_MAX);
}

void motors_set(int16_t speed_a, int16_t speed_b) {
    motor_set(TIM_OC1, MA_IN1_PIN, MA_IN2_PIN, speed_a);
    motor_set(TIM_OC2, MB_IN1_PIN, MB_IN2_PIN, speed_b);
}
