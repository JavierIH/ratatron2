#ifndef IMU_H
#define IMU_H

#include <stdbool.h>
#include <stdint.h>

#define LSM6DSRX_WHO_AM_I_VALUE 0x6B
#define IMU_GYRO_MDPS_PER_LSB   140     // +-4000 dps full scale

// LSM6DSRX on SPI3, gyro at 1666Hz and +-4000 dps. INT1 flags gyro data ready.
bool imu_setup(void);
uint8_t imu_who_am_i(void);
int16_t imu_gyro_z(void);
bool imu_int(void);

#endif
