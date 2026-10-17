#ifndef ENCODERS_H
#define ENCODERS_H

#include <stdint.h>

// Quadrature by hardware: motor A on TIM3, motor B on TIM4
void encoders_setup(void);
int16_t encoder_a(void);
int16_t encoder_b(void);

#endif
