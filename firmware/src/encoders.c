#include <encoders.h>
#include <board.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>


static void encoder_timer_setup(uint32_t timer) {
    timer_set_period(timer, 0xFFFF);
    timer_slave_set_mode(timer, TIM_SMCR_SMS_EM3);  // count on both edges of both channels
    timer_ic_set_input(timer, TIM_IC1, TIM_IC_IN_TI1);
    timer_ic_set_input(timer, TIM_IC2, TIM_IC_IN_TI2);
    timer_ic_set_filter(timer, TIM_IC1, TIM_IC_CK_INT_N_8);
    timer_ic_set_filter(timer, TIM_IC2, TIM_IC_CK_INT_N_8);
    timer_enable_counter(timer);
}

void encoders_setup(void) {
    rcc_periph_clock_enable(RCC_TIM3);
    rcc_periph_clock_enable(RCC_TIM4);

    // PB4 is NJTRST after reset, setting AF2 frees it from JTAG
    gpio_mode_setup(ENCODER_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, ENCODER_PINS);
    gpio_set_af(ENCODER_PORT, GPIO_AF2, ENCODER_PINS);

    encoder_timer_setup(TIM3);
    encoder_timer_setup(TIM4);
}

int16_t encoder_a(void) {
    return (int16_t)timer_get_counter(TIM3);
}

int16_t encoder_b(void) {
    return (int16_t)timer_get_counter(TIM4);
}
