/*
 * Author: Wei Jun
 * Description: Node related methods
 */

#include "../include/node.h"

void node_init(struct Node* node){
    if(node == NULL){
        printf("Malloc failed. Terminating program...\n");
        exit(1);
    }
    else{
        node->location.x = 0;
        node->location.y = 0;
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