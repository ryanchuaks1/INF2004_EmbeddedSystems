/*
 * Author: Leung Wei Jun
 * Description: TODO: Add description
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

    // N S E W
    // 1 1 1 0
    uint8_t is_walled;

    // BFS properties
    bool marked;

    // A* properties
    float f_cost;
    uint8_t g_cost;
    float h_cost;
    struct Node* parent;

    // LinkedList properties
    struct Node* prev;
    struct Node* next;
};

void node_init(struct Node* node);

#endif