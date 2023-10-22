/*
 * Author: Leung Wei Jun
 * Description: TODO: Add description
 */

#ifndef NODE_H
#define NODE_H

#include "common.h"

#define NORTH 0x8u
#define SOUTH 0x4u
#define EAST 0x2u
#define WEST 0x1u

struct Coordinates{
    uint8_t x;
    uint8_t y;
};

struct Node{
    struct Coordinates location;
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

void node_init(struct Node* node){
    if(node == NULL){
        printf("Malloc failed. Terminating program...\n");
        exit(1);
    }
    else{
        node->location.x = 0;
        node->location.y = 0;
        // N S E W
        // 1 1 1 0
        node->is_walled = 0x0u;

        node->f_cost = 0.0f;
        node->g_cost = 0;
        node->h_cost = 0.0f;
        node->parent = NULL;

        node->marked = false;

        (*node).prev = NULL;
        (*node).next = NULL;
    }
}

#endif