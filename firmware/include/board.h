#ifndef BOARD_H
#define BOARD_H

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>

// Ratatron II pinout (STM32F446RET6, LQFP64)
// Net names match hardware/ratatron2.kicad_sch
//
//  PA0   IMU_INT       in   (EXTI0)
//  PA2   TX2           USART2_TX AF7 -> J5
//  PA3   RX2           USART2_RX AF7 -> J5
//  PA4   BATT_ADC      ADC12_IN4
//  PA8   MA_PWM        TIM1_CH1 AF1
//  PA9   MB_PWM        TIM1_CH2 AF1
//  PA13  SWDIO         J4
//  PA14  SWCLK         J4
//  PB0   E_IR_FL       out
//  PB1   E_IR_FR       out
//  PB3   SWO
//  PB4   MA_CHA        TIM3_CH1 AF2 (FT, encoder at 5V)
//  PB5   MA_CHB        TIM3_CH2 AF2 (FT)
//  PB6   MB_CHA        TIM4_CH1 AF2 (FT)
//  PB7   MB_CHB        TIM4_CH2 AF2 (FT)
//  PB8   LED5          out
//  PB9   LED6          out
//  PB10  M_STBY        out
//  PB12  IMU_CS        out
//  PB13  IMU_SCLK      SPI2_SCK AF5
//  PB14  IMU_MISO      SPI2_MISO AF5
//  PB15  IMU_MOSI      SPI2_MOSI AF5
//  PC0   R_IR_SL       ADC123_IN10
//  PC1   R_IR_SR       ADC123_IN11
//  PC2   R_IR_FL       ADC123_IN12
//  PC3   R_IR_FR       ADC123_IN13
//  PC4   E_IR_SL       out
//  PC5   E_IR_SR       out
//  PC6   MA_IN1        out
//  PC7   MA_IN2        out
//  PC8   MB_IN1        out
//  PC9   MB_IN2        out
//  PC10  LED1          out
//  PC11  LED2          out
//  PC12  LED3          out
//  PC13  BUTTON        in, external 4K7 pull-up, active low
//  PD2   LED4          out
//  PH0   OSC_IN        8MHz HSE
//  PH1   OSC_OUT
//
//  Free: PA1, PA5, PA6, PA7, PA10, PA11, PA12, PA15, PB2

// Serial
#define SERIAL_PORT         GPIOA
#define SERIAL_PINS         (GPIO2 | GPIO3)

// User interface
#define BUTTON_PORT         GPIOC
#define BUTTON_PIN          GPIO13
#define NUM_LEDS            6

// Battery: R24 47K / R25 27K
#define BATT_PORT           GPIOA
#define BATT_PIN            GPIO4
#define BATT_CHANNEL        ADC_CHANNEL4
#define BATT_R_TOP          47
#define BATT_R_BOTTOM       27

// Motor driver TB6612FNG
#define MOTOR_PWM_PORT      GPIOA
#define MOTOR_PWM_PINS      (GPIO8 | GPIO9)
#define MOTOR_DIR_PORT      GPIOC
#define MA_IN1_PIN          GPIO6
#define MA_IN2_PIN          GPIO7
#define MB_IN1_PIN          GPIO8
#define MB_IN2_PIN          GPIO9
#define MOTOR_STBY_PORT     GPIOB
#define MOTOR_STBY_PIN      GPIO10
#define MOTOR_PWM_FREQ      20000

// Encoders
#define ENCODER_PORT        GPIOB
#define ENCODER_PINS        (GPIO4 | GPIO5 | GPIO6 | GPIO7)

// IMU MPU-9250 on SPI2
#define IMU_SPI_PORT        GPIOB
#define IMU_SPI_PINS        (GPIO13 | GPIO14 | GPIO15)
#define IMU_CS_PORT         GPIOB
#define IMU_CS_PIN          GPIO12
#define IMU_INT_PORT        GPIOA
#define IMU_INT_PIN         GPIO0

// Infrared sensors
#define IR_RX_PORT          GPIOC
#define IR_RX_PINS          (GPIO0 | GPIO1 | GPIO2 | GPIO3)

#endif
