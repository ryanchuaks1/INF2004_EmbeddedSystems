/*
 * Author: Ryan Chua
 * Description: Common header file for all files
 * contains common libraries and macros
 */

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "message_buffer.h"

#define MAX_ROW 5
#define MAX_COL 5
#define WALL_MASK 0x0F

#define START_NODE_X 4
#define START_NODE_Y 4

#define END_NODE_X 0
#define END_NODE_Y 0

#define NORTH 0x8u
#define SOUTH 0x4u
#define EAST 0x2u
#define WEST 0x1u

#define COMPONENTS_COUNT 7

enum COMPONENT_NAME{
    BARCODE,
    INFRARED,
    MAGNETOMETER,
    MAPPING,
    MOTOR,
    ULTRASONIC,
    WHEEL_ENCODER
};

struct Component{
    MessageBufferHandle_t* buffer;
    TaskHandle_t* task_handler;
    void* flags;
};

struct Car{
    struct Node** position;
    enum PID_STATE* state;
    struct Node* grid[MAX_ROW][MAX_COL];
    struct Component* components[COMPONENTS_COUNT];
};

#endif // COMMON_H
