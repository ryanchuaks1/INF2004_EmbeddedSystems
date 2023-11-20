/*
 * Author: Rene Low
 * Description: TODO: Add description
 */

#ifndef INFRARED_H
#define INFRARED_H

#include "common.h"

#define IR_WALL_THRESHOLD 1000 // Define IR intensity threshold here
#define LEFT_IR_SENSOR_PIN 28
#define RIGHT_IR_SENSOR_PIN 27
// #define FRONT_IR_SENSOR_PIN 12

// struct Infrared{
//     MessageBufferHandle_t* buffer;
//     TaskHandle_t* task_handler;
// };

struct Sensor_Data{
    uint8_t pin;
    uint8_t status[2];
    struct repeating_timer timer;
};

void sensor_data_init(struct Sensor_Data*);
void ir_sensor_init(uint8_t);
void ir_sensor_enable(struct Sensor_Data*);
int64_t ir_sensor_disable(alarm_id_t, void*);
void infrared_task(void* params);

#endif // IR_SENSOR_H