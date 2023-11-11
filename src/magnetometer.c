/*
 * Author: Joshua Chua
 * Description: Main file for the project
 */
#include "../include/magnetometer.h"

void i2c_init_pins()
{

    // This example will use I2C_CHANNEL on the default SDA and SCL pins (4, 5 on a Pico)
    i2c_init(I2C_CHANNEL, I2C_BAUD_RATE);
    //i2c_set_slave_mode(I2C_CHANNEL, false, 0); // Set the PICO to be master
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

}

void magnetometer_init()
{
    //Before Configuring the Sensor, we need to know the register values (Reference Page 23 of the datasheet)
    /*  Register 0x00 = Configuration A: The data rate specifies how frequently the sensor provides measurements. 0x10 means 15Hz according to the datasheet
    *   Register 0x01 = Configuration B: The gain (sensitivity) of the magnetometer. The gain setting determines the measurement range and sensitivity of the sensor.
    *                                   Set to 0x20 as the lowest sensitivity which is 1.3 Gauss
    *   Register 0x02 = Mode Configuration: Set to continuous conversion mode as 0x00
    *   The other Registers: Stores the LSB and MSB of the X, Y and Z Data
    */

    //So, we just need to configure the fist few registers of the Magnetnometer
    //config_data = {Register Address A, ConfigA, Register Address B, Config B, Mode Register, Continuous_config}
    uint8_t config_sampling_data[2] = {SAMPLE_CONFIG_ADDR, 0x1C};
    i2c_write_blocking(I2C_CHANNEL, MAGNETOMETER_ADDR, config_sampling_data, 2, false);

    uint8_t config_gain_data[2] =  {GAIN_CONFIG_ADDR, 0x20};
    i2c_write_blocking(I2C_CHANNEL, MAGNETOMETER_ADDR, config_gain_data, 2, false);

    uint8_t config_mode_data[2] = {MODE_CONFIG_ADDR, 0x00};
    i2c_write_blocking(I2C_CHANNEL, MAGNETOMETER_ADDR, config_mode_data, 2, false);
}

// void accelerometer_init()
// {
//     //Similar to the magnetometer, we need to know the register values (Reference Page 22 of the datasheet)
//     // Register 0x20 = Modfy Power mode and enabled Axis at page 24
//     uint8_t config_data[2] = {0x20, 0x27};
//     i2c_write_blocking(I2C_CHANNEL, ACCELEROMETER_ADDR, config_data, 2, false);

// }

bool reading_magnetometer(int16_t* result)
{
    uint8_t data[6]; //Use to store the data collected
    uint8_t starting_addr = 0x03; //0x03 is the Data Output X MSB register address. Tells the sensor I want to read from there
    i2c_write_blocking(I2C_CHANNEL, MAGNETOMETER_ADDR, &starting_addr, 1, true); 
    i2c_read_blocking(I2C_CHANNEL, MAGNETOMETER_ADDR, data, 6, false); //Read Register 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 where is X, Z and Y respectively

    // In the data sheet. the data is stored as
    // {x_axis_high_8bits, x_axis_low_8bits, z_axis_high_8bits, z_axis_low_8bits, y_axis_high_8bits, y_axis_low_8bits}
    // Hence, we need to bit shift the low bits by 8 and OR bitwise OR with the low bits
    // Stored in 2's complement, hence I never use unsigned integer 
    result[0] = (data[0] << 8 | data[1]); // x-axis
    result[2] = (data[2] << 8 | data[3]); // z-axis
    result[1] = (data[4] << 8 | data[5]); // y-axis

    //printf("%d, %d, %d\n", x_axis, y_axis, z_axis);

    return true;
}

// bool reading_accelerometer(int16_t* result)
// {
//     int8_t data[6]; //Use to store the data collected
//     uint8_t starting_addr = 0x28; //0x03 is the Data Output X MSB register address. Tells the sensor I want to read from there
//     i2c_write_blocking(I2C_CHANNEL, ACCELEROMETER_ADDR, &starting_addr, 1, true); 
//     i2c_read_blocking(I2C_CHANNEL, ACCELEROMETER_ADDR, data, 6, false); //Read Register 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D where is X, Z and Y respectively

//     //Similar to the Magnetometer, however, this time the bits are arranged, low than high
//     // {x_axis_high_8bits, x_axis_low_8bits, y_axis_high_8bits, y_axis_low_8bits, z_axis_high_8bits, z_axis_low_8bits}
//     result[0] = (data[1] << 8) | data[0]; // x-axis
//     result[1] = (data[3] << 8) | data[2]; // y-axis
//     result[2] = (data[5] << 8) | data[4]; // z-axis

//     return true;
// }

//Calculate the difference between current and previous heading
float read_angle_difference(float curr_heading)
{
    static float prev_heading = 0; 

    float difference = curr_heading - prev_heading;

    prev_heading = curr_heading;

    //printf("Difference: %.2f\n", difference);

    return difference;

}

// North = 0, East = 90, South = 180, West = 270
// Using the Arctangent function formula to calculate
float calculate_angle(int16_t x, int16_t y) { 

    // Calculate the magnetic heading in degrees 
    float heading = (atan2(y, x) * RADIANS_TO_DEGREES); 

    if (heading < 0) { 
        heading += 360.0; 
    }

    //read_angle_difference(heading);

    return heading; 
}

// int main()
// {
//     stdio_init_all();
    
//     //Initialise the Magnetometer and Accelerometer
//     i2c_init_pins();
//     magnetometer_init();
//     //accelerometer_init();

//     while (1)
//     {
//         int16_t read_magnetometer_data[3];
        
//         reading_magnetometer(read_magnetometer_data);

//         //printf("X: %d Y: %d Z: %d\n", read_magnetometer_data[0], read_magnetometer_data[1], read_magnetometer_data[2]);

//         float direction = calculate_angle(read_magnetometer_data[0], read_magnetometer_data[1]);

//         printf("Direction: %.2f C\n", direction);

//         //int16_t read_accelerometer_data[3];
        
//         //reading_accelerometer(read_accelerometer_data);

//         //printf("Acceleration: ( x = %d, y = %d, z = %d)\n\n", read_accelerometer_data[0], read_accelerometer_data[1], read_accelerometer_data[2]);

//         //sleep_ms(2000);
//     }

//     return 0;
// }