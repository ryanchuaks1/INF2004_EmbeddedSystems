#include "../include/common.h"
#include "../include/barcode.h"

#include "../include/wheel_encoder.h"
#include "../include/infrared.h"
#include "../include/main.h"

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
    // else if(gpio == WALL_SENSOR_PIN && events == GPIO_IRQ_EDGE_RISE)
    // {
    //     check_if_wall();
    // }
    else if(gpio == LEFT_IR_SENSOR_PIN && events == GPIO_IRQ_EDGE_RISE)
    {
        wall_detected(LEFT_SENSOR);
    }
    else if(gpio == FRONT_IR_SENSOR_PIN && events == GPIO_IRQ_EDGE_RISE)
    {
        wall_detected(FRONT_SENSOR);
    }
}