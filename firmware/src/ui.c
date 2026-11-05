#include <ui.h>
#include <board.h>
#include <clock.h>

#define BUTTON_DEBOUNCE_MS  20


static const struct pin leds[NUM_LEDS] = LED_PINS;

void ui_setup(void) {
    for (int i = 0; i < NUM_LEDS; i++) {
        gpio_mode_setup(leds[i].port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, leds[i].pin);
        gpio_clear(leds[i].port, leds[i].pin);
    }
    gpio_mode_setup(BUTTON_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, BUTTON_PIN);
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

// Debounced: the state only changes after the pin stays stable for BUTTON_DEBOUNCE_MS
bool button_pressed(void) {
    static bool state = false;
    static bool last_raw = false;
    static uint32_t last_change = 0;

    bool raw = !gpio_get(BUTTON_PORT, BUTTON_PIN);
    uint32_t now = millis();
    if (raw != last_raw) {
        last_raw = raw;
        last_change = now;
    }
    if (now - last_change >= BUTTON_DEBOUNCE_MS) state = raw;
    return state;
}
