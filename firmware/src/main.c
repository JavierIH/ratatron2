#include <analog.h>
#include <board.h>
#include <clock.h>
#include <encoders.h>
#include <fan.h>
#include <imu.h>
#include <motors.h>
#include <sensors.h>
#include <serial.h>
#include <ui.h>


#define PRINT_PERIOD_MS     100
#define MOTOR_TEST_SPEED    300     // 30%
#define MOTOR_TEST_MS       500
#define FAN_TEST_SPEED      300     // 30%
#define FAN_TEST_MS         1000

// Spin each motor both ways and report encoder counts, to check that
// motor wiring, encoder wiring and direction signs all match. Then a short
// run of the suction fan. Bridge current is read at the end of each step;
// at 30% duty the motor current is about 3.3 times that.
static void motor_test(void) {
    const struct {
        const char *name;
        int16_t a, b;
    } steps[] = {
        {"A fwd", MOTOR_TEST_SPEED, 0},
        {"A rev", -MOTOR_TEST_SPEED, 0},
        {"B fwd", 0, MOTOR_TEST_SPEED},
        {"B rev", 0, -MOTOR_TEST_SPEED},
    };

    printf("\n-- motor test --\n");
    motors_enable(true);
    for (unsigned i = 0; i < sizeof(steps) / sizeof(steps[0]); i++) {
        int32_t start_a = encoder_a();
        int32_t start_b = encoder_b();
        motors_set(steps[i].a, steps[i].b);
        delay_ms(MOTOR_TEST_MS);
        uint16_t cur_a = motor_a_ma();
        uint16_t cur_b = motor_b_ma();
        motors_set(0, 0);
        delay_ms(200);
        printf("%s: enc_a %+7ld  enc_b %+7ld  cur %4u %4u mA  fault %d %d\n", steps[i].name,
               (long)(encoder_a() - start_a), (long)(encoder_b() - start_b),
               cur_a, cur_b, motor_fault_a(), motor_fault_b());
    }
    motors_enable(false);

    printf("fan %d%%\n", FAN_TEST_SPEED / 10);
    fan_set(FAN_TEST_SPEED);
    delay_ms(FAN_TEST_MS);
    fan_set(0);
    printf("-- end --\n\n");
}

static void startup_leds(void) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds_set_mask(1 << i);
        delay_ms(80);
    }
    leds_set_mask(0);
}

int main(void) {
    clock_setup();
    serial_setup();
    ui_setup();
    analog_setup();
    sensors_setup();
    motors_setup();
    fan_setup();
    encoders_setup();

    printf("\nRatatron II\n");
    startup_leds();

    bool imu_ok = imu_setup();
    printf("IMU who_am_i: 0x%02X (%s)\n", imu_who_am_i(), imu_ok ? "ok" : "FAIL");
    printf("Battery: %u mV\n", battery_mv());
    printf("Press the button to run the motor and fan test\n\n");

    struct ir_reading ir[NUM_IR_SENSORS];
    uint32_t last_print = 0;
    bool last_button = false;

    while (1) {
        bool button = button_pressed();
        if (button && !last_button) motor_test();
        last_button = button;

        if (millis() - last_print >= PRINT_PERIOD_MS) {
            last_print = millis();
            sensors_read(ir);
            int16_t gz = imu_gyro_z();
            printf("bat %5u mV | ir SL %4d SR %4d FL %4d FR %4d | enc %7ld %7ld | gz %6d (%5ld dps) int %d\n",
                   battery_mv(),
                   ir[IR_SIDE_LEFT].value, ir[IR_SIDE_RIGHT].value,
                   ir[IR_FRONT_LEFT].value, ir[IR_FRONT_RIGHT].value,
                   (long)encoder_a(), (long)encoder_b(),
                   gz, (long)gz * IMU_GYRO_MDPS_PER_LSB / 1000, imu_int());

            led_set(0, (millis() / 500) % 2);  // heartbeat
        }
    }
    return 0;
}
