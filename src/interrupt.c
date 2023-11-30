#include "../include/common.h"
#include "../include/barcode.h"
#include "../include/wheel_encoder.h"

/**
 * interrupt_callback()
 * --------
 * Purpose: Handles the interrupts for different components
 * Arguments: params from interrupt
 * Return: None
 */
void interrupt_callback(uint gpio, uint32_t events) 
{
    if (gpio == LEFT_ENCODER_GPIO && events == GPIO_IRQ_EDGE_RISE)
    {
        //When interrupt occurs at the Left Wheel Encoder
        left_rising_edge_count++;
    }
    else if (gpio == RIGHT_ENCODER_GPIO && events == GPIO_IRQ_EDGE_RISE)
    {
        //When interrupt occurs at the Right Wheel Encoder
        right_rising_edge_count++;
    }
    else if(gpio == WALL_SENSOR_PIN && events == GPIO_IRQ_EDGE_RISE)
    {
        //When interrupt occurs at Wall Sensor IR Sensor
        check_if_wall();
    }
}