#include "../include/common.h"
#include "../include/barcode.h"
#include "../include/wheel_encoder.h"

void interrupt_callback(uint gpio, uint32_t events) 
{
    if (gpio == LEFT_ENCODER_GPIO && events == GPIO_IRQ_EDGE_RISE)
    {
        left_rising_edge_count++;
    }
    else if (gpio == RIGHT_ENCODER_GPIO && events == GPIO_IRQ_EDGE_RISE)
    {
        right_rising_edge_count++;
    }
    else if(gpio == WALL_SENSOR_PIN && events == GPIO_IRQ_EDGE_RISE)
    {
        check_if_wall(false);
    }
}