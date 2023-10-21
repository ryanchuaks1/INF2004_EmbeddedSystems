/*
 * Author: Leung Wei Jun
 * Description: TODO: Add description
 */

#ifndef NODE_H
#define NODE_H

#include "common.h"

#define NORTH 0x1u
#define SOUTH 0x2u
#define EAST 0x4u
#define WEST 0x8u

struct Coordinates{
    uint8_t x;
    uint8_t y;
};

struct Node{
    struct Coordinates location;
    uint8_t is_walled;

    // A* properties
    float f_cost;
    float g_cost;
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

        node->f_cost = -1.0f;
        node->g_cost = -1.0f;
        node->h_cost = -1.0f;
        node->parent = NULL;

        (*node).prev = NULL;
        (*node).next = NULL;
    }
}

#endif