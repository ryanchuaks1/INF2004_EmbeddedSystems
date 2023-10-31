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

void print_text(const char *str);
bool check_wall(enum Direction);
void test_rtos(void *pvParameters);
void map_test();

#endif // MAIN_H