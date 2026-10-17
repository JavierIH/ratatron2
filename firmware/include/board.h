#ifndef BOARD_H
#define BOARD_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>

// Ratatron II pinout (STM32F446RET6, LQFP64)
// Net names match hardware/ratatron2.kicad_sch. Pin = LQFP64 package pin.
//
//  Pin  Port  Net        Function
//   2   PC13  BUTTON     in, external 4K7 pull-up, active low
//   5   PH0   OSC_IN     8MHz HSE
//   6   PH1   OSC_OUT
//   8   PC0   R_IR_SL    ADC123_IN10
//   9   PC1   R_IR_SR    ADC123_IN11
//  10   PC2   R_IR_FL    ADC123_IN12
//  11   PC3   R_IR_FR    ADC123_IN13
//  14   PA0   ENCA_CHA   TIM5_CH1 AF2 (32 bit timer)
//  15   PA1   ENCA_CHB   TIM5_CH2 AF2
//  16   PA2   TX2        USART2_TX AF7 -> J5
//  17   PA3   RX2        USART2_RX AF7 -> J5
//  20   PA4   BATT_ADC   ADC12_IN4
//  22   PA6   MA_CUR     ADC12_IN6, CM1+CM2 of U5 (motor A current)
//  23   PA7   MB_CUR     ADC12_IN7, CM1+CM2 of U1 (motor B current)
//  24   PC4   E_IR_SL    out
//  25   PC5   E_IR_SR    out
//  26   PB0   E_IR_FL    out
//  27   PB1   E_IR_FR    out
//  29   PB10  IMU_INT    in (EXTI10), LSM6DSRX INT1
//  33   PB12  IMU_CS     out
//  34   PB13  IMU_SCK    SPI2_SCK AF5
//  35   PB14  IMU_MISO   SPI2_MISO AF5
//  36   PB15  IMU_MOSI   SPI2_MOSI AF5
//  37   PC6   MA_IN1     TIM8_CH1 AF3
//  38   PC7   MA_IN2     TIM8_CH2 AF3
//  39   PC8   MB_IN1     TIM8_CH3 AF3
//  40   PC9   MB_IN2     TIM8_CH4 AF3
//  41   PA8   M_EN       out, EN1+EN2 of both MP6551
//  42   PA9   MA_FAULT   in, nFAULT of U5
//  43   PA10  MB_FAULT   in, nFAULT of U1
//  44   PA11  FAN_PWM    TIM1_CH4 AF1
//  46   PA13  SWDIO      J4
//  49   PA14  SWCLK      J4
//  50   PA15  LED1       out
//  51   PC10  LED2       out
//  52   PC11  LED3       out
//  53   PC12  LED4       out
//  54   PD2   LED5       out
//  56   PB4   LED6       out
//  61   PB8   ENCB_CHA   TIM2_CH1 AF1 (32 bit timer)
//  62   PB9   ENCB_CHB   TIM2_CH2 AF1
//
//  Not connected: PC14, PC15, PA5, PA12, PB2, PB3 (SWO), PB5, PB6, PB7
//  Reserved: PB5 buzzer (TIM3_CH2)

struct pin {
    uint32_t port;
    uint16_t pin;
};

// Serial
#define SERIAL_PORT         GPIOA
#define SERIAL_PINS         (GPIO2 | GPIO3)

// User interface
#define BUTTON_PORT         GPIOC
#define BUTTON_PIN          GPIO13
#define NUM_LEDS            6
#define LED_PINS            {{GPIOA, GPIO15}, {GPIOC, GPIO10}, {GPIOC, GPIO11}, \
                             {GPIOC, GPIO12}, {GPIOD, GPIO2}, {GPIOB, GPIO4}}

// Battery: R24 47K / R25 27K
#define BATT_PORT           GPIOA
#define BATT_PIN            GPIO4
#define BATT_CHANNEL        ADC_CHANNEL4
#define BATT_R_TOP          47
#define BATT_R_BOTTOM       27

// Motor drivers: 2x MP6551, PWM on IN1/IN2
#define MOTOR_PWM_PORT      GPIOC
#define MOTOR_PWM_PINS      (GPIO6 | GPIO7 | GPIO8 | GPIO9)
#define MOTOR_EN_PORT       GPIOA
#define MOTOR_EN_PIN        GPIO8
#define MOTOR_FAULT_PORT    GPIOA
#define MA_FAULT_PIN        GPIO9
#define MB_FAULT_PIN        GPIO10
#define MOTOR_PWM_FREQ      20000

// Motor current: CM1+CM2 (low side current / 10000) into 4K7 to VDD and 4K7 to GND.
// 1.65V at 0A, ~0.24V/A, RC filtered with 100nF
#define MOTOR_CUR_PORT      GPIOA
#define MOTOR_CUR_PINS      (GPIO6 | GPIO7)
#define MA_CUR_CHANNEL      ADC_CHANNEL6
#define MB_CUR_CHANNEL      ADC_CHANNEL7
#define MOTOR_CUR_RATIO     10000   // low side current / CM current
#define MOTOR_CUR_R_OHM     2350    // 4K7 || 4K7

// Suction fan: Q10 driven by TIM1_CH4
#define FAN_PORT            GPIOA
#define FAN_PIN             GPIO11
#define FAN_PWM_FREQ        20000

// Encoders: A on TIM5, B on TIM2
#define ENCA_PORT           GPIOA
#define ENCA_PINS           (GPIO0 | GPIO1)
#define ENCB_PORT           GPIOB
#define ENCB_PINS           (GPIO8 | GPIO9)

// IMU LSM6DSRX on SPI2
#define IMU_SPI_PORT        GPIOB
#define IMU_SPI_PINS        (GPIO13 | GPIO14 | GPIO15)
#define IMU_CS_PORT         GPIOB
#define IMU_CS_PIN          GPIO12
#define IMU_INT_PORT        GPIOB
#define IMU_INT_PIN         GPIO10

// Infrared sensors: emitters through Q5..Q8, receivers on ADC
#define IR_RX_PORT          GPIOC
#define IR_RX_PINS          (GPIO0 | GPIO1 | GPIO2 | GPIO3)
#define E_IR_SL             {GPIOC, GPIO4}
#define E_IR_SR             {GPIOC, GPIO5}
#define E_IR_FL             {GPIOB, GPIO0}
#define E_IR_FR             {GPIOB, GPIO1}
#define R_IR_SL_CHANNEL     ADC_CHANNEL10
#define R_IR_SR_CHANNEL     ADC_CHANNEL11
#define R_IR_FL_CHANNEL     ADC_CHANNEL12
#define R_IR_FR_CHANNEL     ADC_CHANNEL13

#endif
