#include <encoders.h>
#include <board.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>


static void encoder_timer_setup(uint32_t timer) {
    timer_set_period(timer, 0xFFFFFFFF);
    timer_slave_set_mode(timer, TIM_SMCR_SMS_EM3);  // count on both edges of both channels
    timer_ic_set_input(timer, TIM_IC1, TIM_IC_IN_TI1);
    timer_ic_set_input(timer, TIM_IC2, TIM_IC_IN_TI2);
    timer_ic_set_filter(timer, TIM_IC1, TIM_IC_CK_INT_N_8);
    timer_ic_set_filter(timer, TIM_IC2, TIM_IC_CK_INT_N_8);
    timer_enable_counter(timer);
}

void encoders_setup(void) {
    rcc_periph_clock_enable(RCC_TIM2);
    rcc_periph_clock_enable(RCC_TIM5);

    // Pull-ups keep the count still with an encoder unplugged
    gpio_mode_setup(ENCA_CHA_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ENCA_CHA_PIN);
    gpio_set_af(ENCA_CHA_PORT, GPIO_AF1, ENCA_CHA_PIN);
    gpio_mode_setup(ENCA_CHB_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ENCA_CHB_PIN);
    gpio_set_af(ENCA_CHB_PORT, GPIO_AF1, ENCA_CHB_PIN);
    gpio_mode_setup(ENCB_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ENCB_PINS);
    gpio_set_af(ENCB_PORT, GPIO_AF2, ENCB_PINS);

    encoder_timer_setup(TIM2);
    encoder_timer_setup(TIM5);
}

int32_t encoder_a(void) {
    return (int32_t)timer_get_counter(TIM2);
}

int32_t encoder_b(void) {
    return (int32_t)timer_get_counter(TIM5);
}
