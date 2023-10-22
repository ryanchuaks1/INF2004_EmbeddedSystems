/*
 * Author: Leung Wei Jun
 * Description: TODO: Add description
 */

#ifndef MAPPING_H
#define MAPPING_H

#include "grid.h"
#include "linkedlist.h"

struct Node* visit_node(struct Node* curr_node, struct Node* node_to_visit){
    if(curr_node == node_to_visit->parent){
        //move to current node
        return node_to_visit;
    }

    struct Node* next_node = visit_node(curr_node, node_to_visit->parent);

    //TODO: Move to next node

    // if(curr_node == NULL){
    //     //TODO: move from whatever outside the grid position to the starting node
    // }

    // curr_node->parent = NULL;
}

void discover_map(struct Node* grid[MAX_ROW][MAX_COL]){
    struct LinkedList* queue = (struct LinkedList*)malloc(sizeof(struct LinkedList));
    struct Node* curr_node = NULL;

    grid[START_NODE_X][START_NODE_Y]->marked = true;
    insertAtTail(queue, grid[START_NODE_X][START_NODE_Y]);
    
    while(!isEmpty(queue)){
        struct Node* node_to_visit = remove_at_head(queue);

        visit_node(curr_node, node_to_visit);
        curr_node = node_to_visit;

        for(uint8_t i = 0; i < 4; i++)
        {
            uint8_t mask = (1 << i);

            if((node_to_visit->is_walled & mask) == 0){
                uint8_t pos_x = node_to_visit->location.x;
                uint8_t pos_y = node_to_visit->location.y;

                if (mask & (NORTH | SOUTH))
                {
                    pos_x += ((mask >> 2) % 2) ? 1 : -1;
                }
                else if(mask & (EAST | WEST))
                {
                    pos_y += (mask % 2) ? -1 : 1;
                }

                if(pos_x < MAX_ROW && pos_y < MAX_COL && !(grid[pos_x][pos_y]->marked)){
                    grid[pos_x][pos_y]->marked = true;
                    grid[pos_x][pos_y]->parent = node_to_visit;
                    insertAtTail(queue, grid[pos_x][pos_y]);
                }

                node_to_visit->is_walled &= ~mask;
            }
        }
    }
}

#endif