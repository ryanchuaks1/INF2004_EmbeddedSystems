/*
 * Author: Ryan Chua
 * Description: Contains functions and definitions for main.c
 */

#ifndef MAIN_H
#define MAIN_H

#include "common.h"
#include "infrared.h"
#include "barcode.h"
#include "grid.h"
#include "pathfinding.h"
#include "mapping.h"
#include "motor.h"

// FreeRTOS only runs on 1 core
#ifndef RUN_FREERTOS_ON_CORE
#define RUN_FREERTOS_ON_CORE 0
#endif

// Priority value, all tasks are set to priority of 1, so round robin is used.
#define TEMP_TASK_PRIORITY				( tskIDLE_PRIORITY + 1UL )
#define MOV_AVG_TASK_PRIORITY			( tskIDLE_PRIORITY + 1UL )
#define PRINT_TASK_PRIORITY				( tskIDLE_PRIORITY + 1UL )
#define AVG_TASK_PRIORITY				( tskIDLE_PRIORITY + 1UL )

// size of the message buffer
#define mbaTASK_MESSAGE_BUFFER_SIZE		( 60 )

void print_text(const char *str);
void check_walls();
void map_test();
void big_brain();
void mapping();

enum PID_STATE
{
    IDLE,      // Robot is stationary and waiting for next move
    TRANSIT,    // Robot is moving
    ADJUST, // Robot is adjusting its position
    BARCODE,   // Robot is scanning a barcode
};

struct Car{
    enum PID_STATE state;
    bool enter;
    bool exit;
};


#endif // MAIN_H