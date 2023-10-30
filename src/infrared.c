/*
 * Author: Rene Low
 * Description: TODO: wall detection
 */

#include "../include/infrared.h"

// Declare the interrupt setup function
void setup_wall_detection_interrupt();

void ir_sensor_init()
{
    gpio_init(LEFT_IR_SENSOR_PIN);
    gpio_set_dir(LEFT_IR_SENSOR_PIN, GPIO_IN);

    gpio_init(RIGHT_IR_SENSOR_PIN);
    gpio_set_dir(RIGHT_IR_SENSOR_PIN, GPIO_IN);

    gpio_init(FRONT_IR_SENSOR_PIN);
    gpio_set_dir(FRONT_IR_SENSOR_PIN, GPIO_IN);
}

// Define the interrupt handler here
void wall_detected_interrupt_handler()
{
    printf("Wall detected! Performing actions...\n");
}

bool ir_sensor_read(enum Direction dir)
{
    switch (dir)
    {
    case LEFT:
        if (gpio_get(LEFT_IR_SENSOR_PIN)) {
            gpio_set_irq_enabled_with_callback(LEFT_IR_SENSOR_PIN, GPIO_IRQ_EDGE_FALL, true, &wall_detected_interrupt_handler);
            return false;
        } else
            return true;
        break;
    case RIGHT:
        if (gpio_get(RIGHT_IR_SENSOR_PIN)) {
            gpio_set_irq_enabled_with_callback(RIGHT_IR_SENSOR_PIN, GPIO_IRQ_EDGE_FALL, true, &wall_detected_interrupt_handler);
            return false;
        } else
            return true;
        break;
    case FRONT:
        if (gpio_get(FRONT_IR_SENSOR_PIN))  {
            gpio_set_irq_enabled_with_callback(FRONT_IR_SENSOR_PIN, GPIO_IRQ_EDGE_FALL, true, &wall_detected_interrupt_handler);
            return false;
        } else
            return true;
        break;
    default:
        return false;
    }
}
