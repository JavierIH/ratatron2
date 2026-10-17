#ifndef SENSORS_H
#define SENSORS_H

#include <stdint.h>

enum ir_sensor {
    IR_SIDE_LEFT,
    IR_SIDE_RIGHT,
    IR_FRONT_LEFT,
    IR_FRONT_RIGHT,
    NUM_IR_SENSORS
};

struct ir_reading {
    uint16_t ambient;   // emitter off
    uint16_t lit;       // emitter on
    int16_t value;      // lit - ambient
};

// IR emitters SFH4550 (through AO3400A Q5..Q8) and phototransistors ST-1KL3A
void sensors_setup(void);
void sensors_read(struct ir_reading readings[NUM_IR_SENSORS]);
void emitter_set(enum ir_sensor sensor, int on);

#endif
