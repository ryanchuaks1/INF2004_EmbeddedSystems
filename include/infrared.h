/*
 * Author: Rene Low
 * Description: TODO: Add description
 */

#ifndef INFRARED_H
#define INFRARED_H

#include "common.h"

#define IR_WALL_THRESHOLD 1000; // Define IR intensity threshold here
#define LEFT_IR_SENSOR_PIN 26
#define RIGHT_IR_SENSOR_PIN 11
#define FRONT_IR_SENSOR_PIN 12


enum Direction {
    LEFT,
    RIGHT,
    FRONT
};

void ir_sensor_init();
bool ir_sensor_read(enum Direction);

#endif // IR_SENSOR_H