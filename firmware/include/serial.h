#ifndef SERIAL_H
#define SERIAL_H

#include <stdio.h>

// USART1 on J5 (PB6 TX, PB7 RX), 115200 8N1. printf() goes here.
void serial_setup(void);

#endif
