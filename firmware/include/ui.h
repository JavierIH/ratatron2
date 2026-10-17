#ifndef UI_H
#define UI_H

#include <stdbool.h>
#include <stdint.h>

// LEDs D2..D7 (LED1..LED6) and push button SW1
void ui_setup(void);
void led_set(uint8_t led, bool on);
void leds_set_mask(uint8_t mask);
bool button_pressed(void);

#endif
