#ifndef MOTORS_H
#define MOTORS_H

#include <stdbool.h>
#include <stdint.h>

#define MOTOR_MAX   1000

// Two MP6551 H-bridges, PWM on TIM1 at 20kHz. Speed range -MOTOR_MAX..MOTOR_MAX,
// 0 brakes. Outputs are high impedance while disabled.
void motors_setup(void);
void motors_enable(bool enable);
void motors_set(int16_t speed_a, int16_t speed_b);
bool motor_fault_a(void);
bool motor_fault_b(void);

#endif
