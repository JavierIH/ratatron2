#include <sensors.h>
#include <analog.h>
#include <board.h>
#include <clock.h>


// Time for the phototransistor to settle after switching the emitter
#define EMITTER_SETTLE_US   40

struct ir_channel {
    uint32_t emitter_port;
    uint16_t emitter_pin;
    uint8_t adc_channel;
};

static const struct ir_channel channels[NUM_IR_SENSORS] = {
    [IR_SIDE_LEFT]   = {GPIOC, GPIO4, ADC_CHANNEL10},  // E_IR_SL / R_IR_SL
    [IR_SIDE_RIGHT]  = {GPIOC, GPIO5, ADC_CHANNEL11},  // E_IR_SR / R_IR_SR
    [IR_FRONT_LEFT]  = {GPIOB, GPIO0, ADC_CHANNEL12},  // E_IR_FL / R_IR_FL
    [IR_FRONT_RIGHT] = {GPIOB, GPIO1, ADC_CHANNEL13},  // E_IR_FR / R_IR_FR
};

void sensors_setup(void) {
    for (int i = 0; i < NUM_IR_SENSORS; i++) {
        gpio_mode_setup(channels[i].emitter_port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                        channels[i].emitter_pin);
        gpio_clear(channels[i].emitter_port, channels[i].emitter_pin);
    }
}

void emitter_set(enum ir_sensor sensor, int on) {
    const struct ir_channel *ch = &channels[sensor];
    if (on) gpio_set(ch->emitter_port, ch->emitter_pin);
    else gpio_clear(ch->emitter_port, ch->emitter_pin);
}

// One sensor at a time so neighbours don't light each other
void sensors_read(struct ir_reading readings[NUM_IR_SENSORS]) {
    for (int i = 0; i < NUM_IR_SENSORS; i++) {
        readings[i].ambient = analog_read(channels[i].adc_channel);
        emitter_set(i, 1);
        delay_us(EMITTER_SETTLE_US);
        readings[i].lit = analog_read(channels[i].adc_channel);
        emitter_set(i, 0);
        readings[i].value = readings[i].lit - readings[i].ambient;
        delay_us(EMITTER_SETTLE_US);
    }
}
