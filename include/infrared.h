/*
 * Author: Rene Low
 * Description: TODO: Add description
 */

#ifndef INFRARED_H
#define INFRARED_H

#include "common.h"

#define IR_WALL_THRESHOLD 1000 // Define IR intensity threshold here
#define LEFT_IR_SENSOR_PIN 10
#define RIGHT_IR_SENSOR_PIN 11
#define FRONT_IR_SENSOR_PIN 12


enum Direction {
    LEFT,
    RIGHT,
    FRONT
};

struct Sensor_Data{
    uint8_t pin;
    uint8_t status[2];
    struct repeating_timer timer;
};

void sensor_data_init(struct Sensor_Data*);
void ir_sensor_init(uint8_t);
bool ir_sensor_read(enum Direction);
void ir_sensor_enable(struct Sensor_Data*);
int64_t ir_sensor_disable(alarm_id_t, void*);

#endif // IR_SENSOR_H