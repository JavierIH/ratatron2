#ifndef ANALOG_H
#define ANALOG_H

#include <stdint.h>

// ADC1, software triggered single conversions (12 bit)
void analog_setup(void);
uint16_t analog_read(uint8_t channel);

// Battery voltage in mV through R24/R25 divider
uint16_t battery_mv(void);

// Motor current in mA from IPROPI (low side current of each DRV8874). With
// low side braking the same low side MOSFET conducts for the whole PWM period,
// so this is the motor current, not the battery current. No sign: it is only
// right while the current follows the driven direction.
uint16_t motor_a_ma(void);
uint16_t motor_b_ma(void);

#endif
