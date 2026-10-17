#ifndef ENCODERS_H
#define ENCODERS_H

#include <stdint.h>

// Magnetic encoders (ABI quadrature), counted by hardware on the 32 bit
// timers: wheel A on TIM2, wheel B on TIM5. Four counts per pulse.
void encoders_setup(void);
int32_t encoder_a(void);
int32_t encoder_b(void);

#endif
