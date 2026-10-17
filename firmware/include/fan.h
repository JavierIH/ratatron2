#ifndef FAN_H
#define FAN_H

#include <stdint.h>

#define FAN_MAX     1000

// Suction fan through Q10, PWM on TIM1 at 20kHz. Speed range 0..FAN_MAX
void fan_setup(void);
void fan_set(uint16_t speed);

#endif
