/*
 * Author: Leung Wei Jun
 * Description: TODO: Add description
 */

#ifndef MAPPING_H
#define MAPPING_H

#include "grid.h"
#include "linkedlist.h"

struct Node* lowest_common_ancestor(struct LinkedList* path, struct Node* node);
struct LinkedList* generate_path_to_node(struct Node* start_node, struct Node* end_node);
void visit_node(struct Node** car_position, struct Node* node_to_visit);
void discover_map(struct Node* grid[MAX_ROW][MAX_COL], struct Node** car_position);

#endif