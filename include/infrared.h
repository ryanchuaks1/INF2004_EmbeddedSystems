/*
 * Author: Rene Low
 * Description: TODO: Add description
 */

#ifndef INFRARED_H
#define INFRARED_H

#include "common.h"

#define IR_WALL_THRESHOLD 1000; // Define IR intensity threshold here
#define LEFT_IR_SENSOR_PIN 21
#define RIGHT_IR_SENSOR_PIN 20
#define FRONT_IR_SENSOR_PIN 13

// struct Infrared{
//     MessageBufferHandle_t* buffer;
//     TaskHandle_t* task_handler;
// };

enum Direction {
    LEFT,
    RIGHT,
    FRONT
};

void ir_sensor_init();
bool ir_sensor_read(enum Direction);
void wall_detected_interrupt_handler();
void infrared_task(void* params);

#endif // IR_SENSOR_H