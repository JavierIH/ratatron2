#ifndef MOTORS_H
#define MOTORS_H

#include <stdbool.h>
#include <stdint.h>

#define MOTOR_MAX   1000

// TB6612FNG, PWM on TIM1 at 20kHz. Speed range -MOTOR_MAX..MOTOR_MAX
void motors_setup(void);
void motors_enable(bool enable);
void motors_set(int16_t speed_a, int16_t speed_b);

#endif
