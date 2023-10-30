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