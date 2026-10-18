#include <clock.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/dwt.h>
#include <libopencm3/cm3/nvic.h>


static volatile uint32_t ticks = 0;

void sys_tick_handler(void) {
    ticks++;
}

void clock_setup(void) {
    // 16MHz crystal -> 180MHz SYSCLK, APB1 45MHz, APB2 90MHz
    rcc_clock_setup_pll(&rcc_hse_16mhz_3v3[RCC_CLOCK_3V3_180MHZ]);

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_GPIOD);

    systick_set_frequency(1000, rcc_ahb_frequency);  // 1 ms
    systick_counter_enable();
    systick_interrupt_enable();

    dwt_enable_cycle_counter();
}

uint32_t millis(void) {
    return ticks;
}

void delay_ms(uint32_t ms) {
    uint32_t start = ticks;
    while (ticks - start < ms);
}

void delay_us(uint32_t us) {
    uint32_t start = dwt_read_cycle_counter();
    uint32_t cycles = us * (rcc_ahb_frequency / 1000000);
    while (dwt_read_cycle_counter() - start < cycles);
}
