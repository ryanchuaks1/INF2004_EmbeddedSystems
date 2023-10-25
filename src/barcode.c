/*
 * Author: Ryan Chua
 * Description: TODO: Add description
 */

#include "../include/barcode.h"

void barcode_init()
{
    printf("initializing barcode\n");
    // Initialize the ADC
    adc_init();
    adc_gpio_init(BARCODE_SENSOR_PIN);
    adc_select_input(BARCODE_ADC_CHANNEL);
    
    gpio_set_irq_enabled_with_callback(WALL_SENSOR_PIN, GPIO_IRQ_EDGE_RISE, true, &check_if_wall); // enable rising edge interrupt
}

int get_ir_reading()
{
    uint16_t reading = adc_read();
    return reading;
}

void check_if_wall()
{
    if (time_us_64() - last_button_press_time > DEBOUNCE_DELAY_MS * 1000)
    {
        barcodeFlags.count++;
        last_button_press_time = time_us_64(); // update last button press time

        printf("Count: %d\n", barcodeFlags.count);
        printf("Wall detected\n");
    }
}
