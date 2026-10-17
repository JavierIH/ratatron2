#include <imu.h>
#include <board.h>
#include <clock.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>


#define REG_INT1_CTRL       0x0D
#define REG_WHO_AM_I        0x0F
#define REG_CTRL1_XL        0x10
#define REG_CTRL2_G         0x11
#define REG_CTRL3_C         0x12
#define REG_CTRL4_C         0x13
#define REG_CTRL9_XL        0x18
#define REG_OUTZ_L_G        0x26
#define READ_FLAG           0x80

#define CTRL3_C_SW_RESET    0x01
#define RESET_TIMEOUT_MS    10

static void cs_low(void) {
    gpio_clear(IMU_CS_PORT, IMU_CS_PIN);
}

static void cs_high(void) {
    gpio_set(IMU_CS_PORT, IMU_CS_PIN);
}

static void write_reg(uint8_t reg, uint8_t value) {
    cs_low();
    spi_xfer(SPI3, reg);
    spi_xfer(SPI3, value);
    cs_high();
}

static uint8_t read_reg(uint8_t reg) {
    cs_low();
    spi_xfer(SPI3, reg | READ_FLAG);
    uint8_t value = spi_xfer(SPI3, 0x00);
    cs_high();
    return value;
}

bool imu_setup(void) {
    rcc_periph_clock_enable(RCC_SPI3);

    gpio_mode_setup(IMU_CS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, IMU_CS_PIN);
    cs_high();
    gpio_mode_setup(IMU_INT_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, IMU_INT_PIN);

    gpio_mode_setup(IMU_SPI_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, IMU_SPI_PINS);
    gpio_set_af(IMU_SPI_PORT, GPIO_AF6, IMU_SPI_PINS);

    // Up to 10MHz: 45MHz / 8 = 5.6MHz. Mode 3.
    spi_init_master(SPI3, SPI_CR1_BAUDRATE_FPCLK_DIV_8,
                    SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE,
                    SPI_CR1_CPHA_CLK_TRANSITION_2,
                    SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);
    spi_enable_software_slave_management(SPI3);
    spi_set_nss_high(SPI3);
    spi_enable(SPI3);

    write_reg(REG_CTRL3_C, CTRL3_C_SW_RESET);
    uint32_t start = millis();
    while ((read_reg(REG_CTRL3_C) & CTRL3_C_SW_RESET) && millis() - start < RESET_TIMEOUT_MS);

    write_reg(REG_CTRL3_C, 0x44);       // block data update, register auto increment
    write_reg(REG_CTRL4_C, 0x04);       // disable I2C, SPI only
    write_reg(REG_CTRL9_XL, 0xE2);      // disable I3C, keep DEN defaults
    write_reg(REG_CTRL1_XL, 0x40);      // accelerometer 104Hz, +-2g
    write_reg(REG_CTRL2_G, 0x81);       // gyro 1666Hz, +-4000 dps
    write_reg(REG_INT1_CTRL, 0x02);     // gyro data ready on INT1

    return imu_who_am_i() == LSM6DSRX_WHO_AM_I_VALUE;
}

uint8_t imu_who_am_i(void) {
    return read_reg(REG_WHO_AM_I);
}

int16_t imu_gyro_z(void) {
    cs_low();
    spi_xfer(SPI3, REG_OUTZ_L_G | READ_FLAG);
    uint8_t low = spi_xfer(SPI3, 0x00);
    uint8_t high = spi_xfer(SPI3, 0x00);
    cs_high();
    return (int16_t)((high << 8) | low);
}

bool imu_int(void) {
    return gpio_get(IMU_INT_PORT, IMU_INT_PIN);
}
