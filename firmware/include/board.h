#ifndef BOARD_H
#define BOARD_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>

// Ratatron II pinout (STM32F446RET6, LQFP64)
// Net names match hardware/ratatron2.kicad_sch. Pin = LQFP64 package pin.
// IC1 sits at rot 180 in the nose: pins 17-32 face the front edge, 33-48 the
// left IR, 49-64 the fan (rear) and 1-16 the right IR.
//
//  Pin  Port  Net        Function
//   2   PC13  BUTTON     in, external 4K7 pull-up, active low
//   3   PC14  MB_FAULT   in, nFAULT of U1
//   5   PH0   OSC_IN     16MHz HSE
//   6   PH1   OSC_OUT
//   8   PC0   LED6       out
//   9   PC1   MB_CUR     ADC123_IN11, IPROPI of U1 (motor B current)
//  10   PC2   R_IR_FR    ADC123_IN12
//  11   PC3   E_IR_FR    out
//  14   PA0   ENCB_CHA   TIM5_CH1 AF2 (32 bit timer)
//  15   PA1   ENCB_CHB   TIM5_CH2 AF2
//  16   PA2   E_IR_SR    out
//  17   PA3   R_IR_SR    ADC123_IN3
//  24   PC4   BATT_ADC   ADC12_IN14
//  25   PC5   MA_CUR     ADC12_IN15, IPROPI of U5 (motor A current)
//  26   PB0   R_IR_FL    ADC12_IN8
//  27   PB1   R_IR_SL    ADC12_IN9
//  33   PB12  E_IR_SL    out
//  34   PB13  E_IR_FL    out
//  35   PB14  LED1       out
//  36   PB15  LED2       out
//  37   PC6   LED3       out
//  38   PC7   MA_FAULT   in, nFAULT of U5
//  41   PA8   MA_IN1     TIM1_CH1 AF1
//  42   PA9   MA_IN2     TIM1_CH2 AF1
//  43   PA10  MB_IN1     TIM1_CH3 AF1
//  44   PA11  MB_IN2     TIM1_CH4 AF1
//  45   PA12  M_EN       out, nSLEEP of both DRV8874
//  46   PA13  SWDIO      J4
//  49   PA14  SWCLK      J4
//  50   PA15  ENCA_CHA   TIM2_CH1 AF1 (32 bit timer)
//  51   PC10  IMU_SCK    SPI3_SCK AF6
//  52   PC11  IMU_MISO   SPI3_MISO AF6
//  53   PC12  IMU_MOSI   SPI3_MOSI AF6
//  54   PD2   IMU_CS     out
//  55   PB3   ENCA_CHB   TIM2_CH2 AF1
//  56   PB4   IMU_INT    in (EXTI4), LSM6DSRX INT1
//  57   PB5   FAN_PWM    TIM3_CH2 AF2
//  58   PB6   TX1        USART1_TX AF7 -> J5
//  59   PB7   RX1        USART1_RX AF7 -> J5
//  61   PB8   LED4       out
//  62   PB9   LED5       out
//
//  Not connected: PC15, PA4, PA5, PA6, PA7, PB2, PB10, PC8, PC9
//  Reserved: PC9 buzzer (TIM8_CH4)

struct pin {
    uint32_t port;
    uint16_t pin;
};

// Serial
#define SERIAL_PORT         GPIOB
#define SERIAL_PINS         (GPIO6 | GPIO7)

// User interface
#define BUTTON_PORT         GPIOC
#define BUTTON_PIN          GPIO13
#define NUM_LEDS            6
#define LED_PINS            {{GPIOB, GPIO14}, {GPIOB, GPIO15}, {GPIOC, GPIO6}, \
                             {GPIOB, GPIO8}, {GPIOB, GPIO9}, {GPIOC, GPIO0}}

// Battery: R24 47K / R25 27K
#define BATT_PORT           GPIOC
#define BATT_PIN            GPIO4
#define BATT_CHANNEL        ADC_CHANNEL14
#define BATT_R_TOP          47
#define BATT_R_BOTTOM       27

// Motor drivers: 2x DRV8874 in PWM mode (PMODE = VDD), PWM on IN1/IN2
#define MOTOR_PWM_PORT      GPIOA
#define MOTOR_PWM_PINS      (GPIO8 | GPIO9 | GPIO10 | GPIO11)
#define MOTOR_EN_PORT       GPIOA
#define MOTOR_EN_PIN        GPIO12
#define MOTOR_FAULT_PORT    GPIOC
#define MA_FAULT_PIN        GPIO7
#define MB_FAULT_PIN        GPIO14
#define MOTOR_PWM_FREQ      20000

// Motor current: IPROPI (455uA per A of low side current) into 2K2 to GND,
// 0V at 0A, ~1V/A, 10nF filter. VREF = VDD, so each driver chops above ~3.3A
#define MOTOR_CUR_PORT      GPIOC
#define MOTOR_CUR_PINS      (GPIO5 | GPIO1)
#define MA_CUR_CHANNEL      ADC_CHANNEL15
#define MB_CUR_CHANNEL      ADC_CHANNEL11
#define MOTOR_CUR_UA_PER_A  455     // IPROPI current per A of motor current
#define MOTOR_CUR_R_OHM     2200    // R32, R34

// Suction fan: Q10 driven by TIM3_CH2
#define FAN_PORT            GPIOB
#define FAN_PIN             GPIO5
#define FAN_PWM_FREQ        20000

// Encoders: A on TIM2 (channels on two ports), B on TIM5
#define ENCA_CHA_PORT       GPIOA
#define ENCA_CHA_PIN        GPIO15
#define ENCA_CHB_PORT       GPIOB
#define ENCA_CHB_PIN        GPIO3
#define ENCB_PORT           GPIOA
#define ENCB_PINS           (GPIO0 | GPIO1)

// IMU LSM6DSRX on SPI3
#define IMU_SPI_PORT        GPIOC
#define IMU_SPI_PINS        (GPIO10 | GPIO11 | GPIO12)
#define IMU_CS_PORT         GPIOD
#define IMU_CS_PIN          GPIO2
#define IMU_INT_PORT        GPIOB
#define IMU_INT_PIN         GPIO4

// Infrared sensors: emitters through Q5..Q8, receivers on ADC
#define E_IR_SL             {GPIOB, GPIO12}
#define E_IR_SR             {GPIOA, GPIO2}
#define E_IR_FL             {GPIOB, GPIO13}
#define E_IR_FR             {GPIOC, GPIO3}
#define R_IR_SL             {GPIOB, GPIO1}
#define R_IR_SR             {GPIOA, GPIO3}
#define R_IR_FL             {GPIOB, GPIO0}
#define R_IR_FR             {GPIOC, GPIO2}
#define R_IR_SL_CHANNEL     ADC_CHANNEL9
#define R_IR_SR_CHANNEL     ADC_CHANNEL3
#define R_IR_FL_CHANNEL     ADC_CHANNEL8
#define R_IR_FR_CHANNEL     ADC_CHANNEL12

#endif
