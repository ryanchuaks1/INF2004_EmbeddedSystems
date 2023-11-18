/*
 * Author: Ryan Chua
 * Description: Contains functions and definitions for main.c
 */

#ifndef MAIN_H
#define MAIN_H

//#include "common.h"

#include "barcode.h"
#include "infrared.h"
#include "magnetometer.h"
#include "motor.h"
#include "ultrasonic.h"
#include "wheel_encoder.h"

#include "node.h"
#include "grid.h"
#include "pathfinding.h"
#include "mapping.h"


// FreeRTOS only runs on 1 core
#ifndef RUN_FREERTOS_ON_CORE
#define RUN_FREERTOS_ON_CORE 0
#endif

// Priority value, all tasks are set to priority of 1, so round robin is used.
#define MAIN_TASK_PRIORITY				    ( tskIDLE_PRIORITY + 2UL )
#define BARCODE_TASK_PRIORITY				    ( tskIDLE_PRIORITY + 1UL )
#define INFRARED_TASK_PRIORITY			        ( tskIDLE_PRIORITY + 1UL )
#define MAGNETOMETER_TASK_PRIORITY				( tskIDLE_PRIORITY + 1UL )
#define MAPPING_TASK_PRIORITY				    ( tskIDLE_PRIORITY + 1UL )
#define MOTOR_TASK_PRIORITY				        ( tskIDLE_PRIORITY + 1UL )
#define ULTRASONIC_TASK_PRIORITY				( tskIDLE_PRIORITY + 1UL )
#define WHEEL_ENCODER_TASK_PRIORITY				( tskIDLE_PRIORITY + 1UL )

// size of the message buffer
#define mbaTASK_MESSAGE_BUFFER_SIZE		( 60 )

//void print_text(const char *str);
void state_enter(struct Car* car);
void state_execute(struct Car* car);
void state_exit(struct Car* car);
void change_state(struct Car* car, enum PID_STATE next_state);
void components_init(struct Component* components[COMPONENTS_COUNT]);
void component_init(struct Component* component);
void main_task(void* params);
void car_init(struct Car* car);


enum PID_STATE
{
    IDLE,      // Robot is stationary and waiting for next move
    TRANSIT,    // Robot is moving
    ADJUST, // Robot is adjusting its position
    SCANNING,   // Robot is scanning a barcode
};


#endif // MAIN_H