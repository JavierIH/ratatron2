#ifndef ANALOG_H
#define ANALOG_H

#include <stdint.h>

// ADC1, software triggered single conversions (12 bit)
void analog_setup(void);
uint16_t analog_read(uint8_t channel);

// Battery voltage in mV through R24/R25 divider
uint16_t battery_mv(void);

// Bridge current in mA: low side current of each MP6551, averaged by the RC
// filter over several PWM periods. With high side braking this is the current
// taken from the battery; the motor current is about this / duty. Current
// flowing back to the battery reads 0. Zero is measured in analog_setup().
uint16_t motor_a_ma(void);
uint16_t motor_b_ma(void);

#endif
