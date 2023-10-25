#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Define the I2C address of the GY-511 magnetometer (FIxed Value by Hardware, Cannot change)
#define MAGNETOMETER_ADDR 0x1E
#define I2C_BRAUD_RATE 100000 

#define I2C_CHANNEL i2c0
#define I2C_SDA_PIN 0
#define I2C_SCL_PIN 1

#define SAMPLE_CONFIG_ADDR 0x00
#define GAIN_CONFIG_ADDR 0x01
#define MODE_CONFIG_ADDR 0x03


void magnetometer_init()
{
    // Use Default Definitions for I2C0 Initailisation
#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
    #warning i2c/ht16k33_i2c example requires a board with I2C pins
#else
    // This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)
    i2c_init(i2c_default, I2C_BRAUD_RATE);
    i2c_set_slave_mode(i2c0, false, 0); // Set the PICO to be master
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    //Before Configuring the Sensor, we need to know the register values
    /*  Register 0x00 = Configuration A: The data rate specifies how frequently the sensor provides measurements. 0x07 means 15Hz
    *   Register 0x01 = Configuration B: The gain (sensitivity) of the magnetometer. The gain setting determines the measurement range and sensitivity of the sensor.
    *                                   Set to 0x00 as the default
    *   Register 0x02 = Mode Configuration: Set to continuoes measurement as 0x00
    *   The other Registers: Stores the LSB and MSB of the X, Y and Z Data
    */

    //So, we just need to configure the fist few registers of the Magnetnometer
    // config_data = {Register Address A, ConfigA, Register Address B, Config B, Mode Register, Continuous_config}
    uint8_t config_data[6] = {SAMPLE_CONFIG_ADDR, 0x70, GAIN_CONFIG_ADDR, 0x00, MODE_CONFIG_ADDR, 0x00};
    i2c_write_blocking(i2c0, MAGNETOMETER_ADDR, config_data, 6, false);

#endif
}

int8_t* reading_magnetometer()
{
    int8_t data[6]; //Use to store the data collected
    uint8_t starting_addr = 0x03; //0x03 is the Data Output X MSB register address. Tells the sensor I want to read from there
    i2c_write_blocking(i2c0, MAGNETOMETER_ADDR, &starting_addr, 1, true); 
    i2c_read_blocking(i2c0, MAGNETOMETER_ADDR, data, 6, false); //Read Register 0x03, 0x04, 0x05, 0x06, 0x07 0x08 where is X, Z and Y respectively

    // Some Formula Found Online to convert the data from the Magnetometer
    int8_t x_axis = (data[0] << 8) | data[1];
    int8_t z_axis = (data[2] << 8) | data[3];
    int8_t y_axis = (data[4] << 8) | data[5];

    //printf("X: %d Y: %d Z: %d\n", x_axis, y_axis, z_axis);

    int8_t result[3] = {x_axis, z_axis, y_axis};

    return result;
}

int main()
{
    stdio_init_all();

    magnetometer_init();

    while (1)
    {
        int8_t* readings = reading_magnetometer();

        printf("X: %d Y: %d Z: %d\n", readings[0], readings[1], readings[2]);

        sleep_ms(1000);
    }

    return 0;
}