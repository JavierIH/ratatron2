#ifndef IMU_H
#define IMU_H

#include <stdbool.h>
#include <stdint.h>

#define MPU9250_WHO_AM_I_VALUE  0x71
#define MPU9255_WHO_AM_I_VALUE  0x73

// MPU-9250 on SPI2, gyro at +-2000 dps (16.4 LSB/dps)
bool imu_setup(void);
uint8_t imu_who_am_i(void);
int16_t imu_gyro_z(void);
bool imu_int(void);

#endif
