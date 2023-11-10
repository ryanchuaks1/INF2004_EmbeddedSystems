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
#include "movement.h"

void print_text(const char *str);
void check_walls();
void map_test();
void big_brain();
void mapping();

enum robot_state
{
    IDLE,      // Robot is stationary and waiting for next move
    MOVING,    // Robot is moving
    ADJUSTING, // Robot is adjusting its position
    BARCODE,   // Robot is scanning a barcode
};


static enum robot_state state = IDLE;

#endif // MAIN_H\