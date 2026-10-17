#ifndef SERIAL_H
#define SERIAL_H

#include <stdio.h>

// USART2 on J5 (PA2 TX, PA3 RX), 115200 8N1. printf() goes here.
void serial_setup(void);

#endif
