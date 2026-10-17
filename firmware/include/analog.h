#ifndef ANALOG_H
#define ANALOG_H

#include <stdint.h>

// ADC1, software triggered single conversions (12 bit)
void analog_setup(void);
uint16_t analog_read(uint8_t channel);

// Battery voltage in mV through R24/R25 divider
uint16_t battery_mv(void);

#endif
