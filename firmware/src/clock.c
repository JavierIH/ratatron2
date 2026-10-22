#include <clock.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/pwr.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/dwt.h>
#include <libopencm3/cm3/nvic.h>


// Over-drive bits, not defined in libopencm3 (RM0390 PWR_CR / PWR_CSR)
#define PWR_CR_ODEN         (1 << 16)
#define PWR_CR_ODSWEN       (1 << 17)
#define PWR_CSR_ODRDY       (1 << 16)
#define PWR_CSR_ODSWRDY     (1 << 17)

static volatile uint32_t ticks = 0;

void sys_tick_handler(void) {
    ticks++;
}

// Same steps as rcc_clock_setup_pll() plus the over-drive mode, needed above
// 168MHz (RM0390 5.1.4): enabled once the PLL is on, before switching to it
static void pll_setup_overdrive(const struct rcc_clock_scale *clock) {
    rcc_osc_on(RCC_HSI);
    rcc_wait_for_osc_ready(RCC_HSI);
    rcc_set_sysclk_source(RCC_CFGR_SW_HSI);

    rcc_osc_on(RCC_HSE);
    rcc_wait_for_osc_ready(RCC_HSE);

    rcc_periph_clock_enable(RCC_PWR);
    pwr_set_vos_scale(clock->voltage_scale);

    rcc_set_hpre(clock->hpre);
    rcc_set_ppre1(clock->ppre1);
    rcc_set_ppre2(clock->ppre2);

    rcc_osc_off(RCC_PLL);
    rcc_set_main_pll_hse(clock->pllm, clock->plln, clock->pllp, clock->pllq, clock->pllr);
    rcc_osc_on(RCC_PLL);

    PWR_CR |= PWR_CR_ODEN;
    while (!(PWR_CSR & PWR_CSR_ODRDY));
    PWR_CR |= PWR_CR_ODSWEN;
    while (!(PWR_CSR & PWR_CSR_ODSWRDY));

    rcc_wait_for_osc_ready(RCC_PLL);

    flash_dcache_enable();
    flash_icache_enable();
    flash_set_ws(clock->flash_config);

    rcc_set_sysclk_source(RCC_CFGR_SW_PLL);
    rcc_wait_for_sysclk_status(RCC_PLL);

    rcc_ahb_frequency = clock->ahb_frequency;
    rcc_apb1_frequency = clock->apb1_frequency;
    rcc_apb2_frequency = clock->apb2_frequency;

    rcc_osc_off(RCC_HSI);
}

void clock_setup(void) {
    // 16MHz crystal -> 180MHz SYSCLK, APB1 45MHz, APB2 90MHz
    pll_setup_overdrive(&rcc_hse_16mhz_3v3[RCC_CLOCK_3V3_180MHZ]);

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
