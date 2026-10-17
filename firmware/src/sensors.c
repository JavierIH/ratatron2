#include <sensors.h>
#include <analog.h>
#include <board.h>
#include <clock.h>


// Time for the phototransistor to settle after switching the emitter
#define EMITTER_SETTLE_US   40

struct ir_channel {
    struct pin emitter;
    uint8_t adc_channel;
};

static const struct ir_channel channels[NUM_IR_SENSORS] = {
    [IR_SIDE_LEFT]   = {E_IR_SL, R_IR_SL_CHANNEL},
    [IR_SIDE_RIGHT]  = {E_IR_SR, R_IR_SR_CHANNEL},
    [IR_FRONT_LEFT]  = {E_IR_FL, R_IR_FL_CHANNEL},
    [IR_FRONT_RIGHT] = {E_IR_FR, R_IR_FR_CHANNEL},
};

void sensors_setup(void) {
    for (int i = 0; i < NUM_IR_SENSORS; i++) {
        gpio_mode_setup(channels[i].emitter.port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                        channels[i].emitter.pin);
        gpio_clear(channels[i].emitter.port, channels[i].emitter.pin);
    }
}

void emitter_set(enum ir_sensor sensor, int on) {
    const struct pin *e = &channels[sensor].emitter;
    if (on) gpio_set(e->port, e->pin);
    else gpio_clear(e->port, e->pin);
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
