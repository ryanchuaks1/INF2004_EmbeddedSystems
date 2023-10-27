/*
 * Author: Leung Wei Jun
 * Description: TODO: Add description
 */

#ifndef GRID_H
#define GRID_H

#include "common.h"
#include "node.h"

void grid_init(struct Node* grid[MAX_ROW][MAX_COL]);
void print_grid(struct Node* grid[MAX_ROW][MAX_COL]);
void add_wall(struct Node* grid[MAX_ROW][MAX_COL], struct Node* node, uint8_t wall_direction);

#endif


