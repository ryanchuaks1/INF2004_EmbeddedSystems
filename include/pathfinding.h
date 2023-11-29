/*
 * Author: Leung Wei Jun
 * Description: A* Pathfinding implementation on C
 */

#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "common.h"
#include "linkedlist.h"
#include "sorting.h"

#define PATH_COST 10
// #define DIAG_PATH_COST 14.142135623730950488016887242097f

struct LinkedList* generate_path(struct Node* end_node, struct Node* start_node);
void sort_f_cost(struct LinkedList* ll);
uint8_t calculate_g_cost(struct Node* curr_node, struct Node* start_node);
float calculate_h_cost(struct Node* curr_node, struct Node* end_node);
struct LinkedList* compute_path(struct Node* grid[MAX_ROW][MAX_COL], struct Node* start_node, struct Node* end_node);
void pathfinding_task(void* params);
void print_pathfinding_grid(struct Node* grid[MAX_ROW][MAX_COL], struct LinkedList* path);


#endif