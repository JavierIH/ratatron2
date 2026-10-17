#include <serial.h>
#include <board.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>


void serial_setup(void) {
    rcc_periph_clock_enable(RCC_USART2);

    gpio_mode_setup(SERIAL_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, SERIAL_PINS);
    gpio_set_af(SERIAL_PORT, GPIO_AF7, SERIAL_PINS);

    usart_set_baudrate(USART2, 115200);
    usart_set_databits(USART2, 8);
    usart_set_stopbits(USART2, USART_STOPBITS_1);
    usart_set_mode(USART2, USART_MODE_TX_RX);
    usart_set_parity(USART2, USART_PARITY_NONE);
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
    usart_enable(USART2);

    setvbuf(stdout, NULL, _IONBF, 0);
}

// newlib hook for printf
int _write(int file, char *ptr, int len);
int _write(int file, char *ptr, int len) {
    (void)file;
    for (int i = 0; i < len; i++) {
        if (ptr[i] == '\n') usart_send_blocking(USART2, '\r');
        usart_send_blocking(USART2, ptr[i]);
    }
    return len;
}
