/*
 * Author: Ryan Chua
 * Description: TODO: Add description
 */

#include "../include/barcode.h"

void barcode_init()
{
    adc_init(); // Initialize the ADC
    adc_gpio_init(BARCODE_SENSOR_PIN); // Use GP5 (GPIO pin 5) as the input for the ADC
    adc_select_input(BARCODE_ADC_CHANNEL); // Select ADC channel 2 (A2)
}

// Read the intensity of the infrared sensor
int read_intensity()
{
    // Read analog data from the infrared sensor using the ADC
    uint16_t raw_value = adc_read(); // Read the ADC value (0-65535)
    return raw_value;
}
