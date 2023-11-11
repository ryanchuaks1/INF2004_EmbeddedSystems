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