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
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "message_buffer.h"

#include "lwip/apps/httpd.h"
#include "pico/cyw43_arch.h"

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
    float wheels_ratio;
    float duty_cycle;
    MessageBufferHandle_t* main_buffer;
};

void interrupt_callback(uint gpio, uint32_t events);
extern struct Car* global_car;
extern char start_delim;
extern char letter;
extern char end_delim;

enum DIRECTION{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

enum SENSOR_DIRECTION {
    LEFT_SENSOR,
    FRONT_SENSOR
};



#endif // COMMON_H
