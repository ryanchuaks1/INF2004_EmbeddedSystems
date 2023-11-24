/*
 * Author: Leung Wei Jun
 * Description: Node struct and its related function prototypes
 */

#ifndef NODE_H
#define NODE_H

#include "common.h"

struct Coordinates{
    uint8_t x;
    uint8_t y;
};

struct Node{
    struct Coordinates location;

    // Used for storing wall information in a node.
    // N S E W
    // 4 2 1 0 -> Decimal/Integer
    // 1 1 1 0 -> If 1 = Wall, 0 = No wall
    uint8_t is_walled;

    // DFS properties
    bool marked;

    // A* properties
    float f_cost;
    uint8_t g_cost;
    float h_cost;
    struct Node* parent;

    // LinkedList properties, pointers to previous and next node
    struct Node* prev;
    struct Node* next;
};

void node_init(struct Node* node);

#endif