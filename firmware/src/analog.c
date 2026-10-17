#include <analog.h>
#include <board.h>
#include <clock.h>

#include <libopencm3/stm32/rcc.h>


#define VREF_MV     3300

void analog_setup(void) {
    rcc_periph_clock_enable(RCC_ADC1);

    gpio_mode_setup(BATT_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, BATT_PIN);
    gpio_mode_setup(IR_RX_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, IR_RX_PINS);
    gpio_mode_setup(MOTOR_CUR_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, MOTOR_CUR_PINS);

    adc_power_off(ADC1);
    adc_set_clk_prescale(ADC_CCR_ADCPRE_BY4);  // 90MHz / 4 = 22.5MHz
    adc_disable_scan_mode(ADC1);
    adc_set_single_conversion_mode(ADC1);
    adc_disable_external_trigger_regular(ADC1);
    adc_set_right_aligned(ADC1);
    adc_set_resolution(ADC1, ADC_CR1_RES_12BIT);
    adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_56CYC);
    adc_power_on(ADC1);
    delay_us(10);
}

uint16_t analog_read(uint8_t channel) {
    adc_set_regular_sequence(ADC1, 1, &channel);
    adc_start_conversion_regular(ADC1);
    while (!adc_eoc(ADC1));
    return adc_read_regular(ADC1);
}

uint16_t battery_mv(void) {
    uint32_t raw = analog_read(BATT_CHANNEL);
    uint32_t pin_mv = raw * VREF_MV / 4095;
    return pin_mv * (BATT_R_TOP + BATT_R_BOTTOM) / BATT_R_BOTTOM;
}
