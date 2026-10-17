#include <imu.h>
#include <board.h>
#include <clock.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>


#define REG_GYRO_CONFIG     0x1B
#define REG_GYRO_ZOUT_H     0x47
#define REG_USER_CTRL       0x6A
#define REG_PWR_MGMT_1      0x6B
#define REG_WHO_AM_I        0x75
#define READ_FLAG           0x80

static void cs_low(void) {
    gpio_clear(IMU_CS_PORT, IMU_CS_PIN);
}

static void cs_high(void) {
    gpio_set(IMU_CS_PORT, IMU_CS_PIN);
}

static void write_reg(uint8_t reg, uint8_t value) {
    cs_low();
    spi_xfer(SPI2, reg);
    spi_xfer(SPI2, value);
    cs_high();
}

static uint8_t read_reg(uint8_t reg) {
    cs_low();
    spi_xfer(SPI2, reg | READ_FLAG);
    uint8_t value = spi_xfer(SPI2, 0x00);
    cs_high();
    return value;
}

bool imu_setup(void) {
    rcc_periph_clock_enable(RCC_SPI2);

    gpio_mode_setup(IMU_CS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, IMU_CS_PIN);
    cs_high();
    gpio_mode_setup(IMU_INT_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, IMU_INT_PIN);

    gpio_mode_setup(IMU_SPI_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, IMU_SPI_PINS);
    gpio_set_af(IMU_SPI_PORT, GPIO_AF5, IMU_SPI_PINS);

    // Registers need SPI <= 1MHz: 45MHz / 64 = 703kHz. Mode 3.
    spi_init_master(SPI2, SPI_CR1_BAUDRATE_FPCLK_DIV_64,
                    SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE,
                    SPI_CR1_CPHA_CLK_TRANSITION_2,
                    SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);
    spi_enable_software_slave_management(SPI2);
    spi_set_nss_high(SPI2);
    spi_enable(SPI2);

    write_reg(REG_PWR_MGMT_1, 0x80);    // reset
    delay_ms(100);
    write_reg(REG_PWR_MGMT_1, 0x01);    // PLL clock
    write_reg(REG_USER_CTRL, 0x10);     // disable I2C, SPI only
    write_reg(REG_GYRO_CONFIG, 0x18);   // +-2000 dps
    delay_ms(10);

    uint8_t id = imu_who_am_i();
    return id == MPU9250_WHO_AM_I_VALUE || id == MPU9255_WHO_AM_I_VALUE;
}

uint8_t imu_who_am_i(void) {
    return read_reg(REG_WHO_AM_I);
}

int16_t imu_gyro_z(void) {
    cs_low();
    spi_xfer(SPI2, REG_GYRO_ZOUT_H | READ_FLAG);
    uint8_t high = spi_xfer(SPI2, 0x00);
    uint8_t low = spi_xfer(SPI2, 0x00);
    cs_high();
    return (int16_t)((high << 8) | low);
}

bool imu_int(void) {
    return gpio_get(IMU_INT_PORT, IMU_INT_PIN);
}
