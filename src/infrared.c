/*
 * Author: Rene Low
 * Description: TODO: Add description
 */

#include "../include/infrared.h"

void ir_sensor_init()
{
    adc_init();
    adc_gpio_init(LEFT_IR_SENSOR_PIN);
    adc_gpio_init(RIGHT_IR_SENSOR_PIN);
    adc_gpio_init(FRONT_IR_SENSOR_PIN);
    gpio_set_function(1, GPIO_FUNC_SIO); 
    gpio_set_dir(1, GPIO_IN); 
}

bool ir_sensor_read(enum Direction dir)
{
    uint16_t reading = adc_read();
    switch (dir) {
        case LEFT:
            adc_select_input(LEFT_IR_SENSOR_PIN);
            return reading < IR_WALL_THRESHOLD;
        case RIGHT:
            adc_select_input(RIGHT_IR_SENSOR_PIN);
        case FRONT:
            adc_select_input(FRONT_IR_SENSOR_PIN);
        default:
            return false;
        }
    
    printf("Reading: %d\n", reading);
    return reading > IR_WALL_THRESHOLD;
}
