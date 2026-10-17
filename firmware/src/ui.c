#include <ui.h>
#include <board.h>


struct pin {
    uint32_t port;
    uint16_t pin;
};

static const struct pin leds[NUM_LEDS] = {
    {GPIOC, GPIO10},  // LED1
    {GPIOC, GPIO11},  // LED2
    {GPIOC, GPIO12},  // LED3
    {GPIOD, GPIO2},   // LED4
    {GPIOB, GPIO8},   // LED5
    {GPIOB, GPIO9},   // LED6
};

void ui_setup(void) {
    for (int i = 0; i < NUM_LEDS; i++) {
        gpio_mode_setup(leds[i].port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, leds[i].pin);
        gpio_clear(leds[i].port, leds[i].pin);
    }
    // External pull-up R2 and RC debounce on board
    gpio_mode_setup(BUTTON_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, BUTTON_PIN);
}

void led_set(uint8_t led, bool on) {
    if (led >= NUM_LEDS) return;
    if (on) gpio_set(leds[led].port, leds[led].pin);
    else gpio_clear(leds[led].port, leds[led].pin);
}

void leds_set_mask(uint8_t mask) {
    for (int i = 0; i < NUM_LEDS; i++) {
        led_set(i, mask & (1 << i));
    }
}

bool button_pressed(void) {
    return !gpio_get(BUTTON_PORT, BUTTON_PIN);
}
