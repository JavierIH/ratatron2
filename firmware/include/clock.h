#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

void clock_setup(void);
uint32_t millis(void);
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

#endif
