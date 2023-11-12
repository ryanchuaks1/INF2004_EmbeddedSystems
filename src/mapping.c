/*
 * Author: Wei Jun
 * Description: TODO: Add description
 */

#include "../include/mapping.h"

void mapping_task(void* params)
{
    struct Car* car = (struct Car*)params;

    // add_wall(grid, grid[3][3], SOUTH | EAST);
    // add_wall(grid, grid[2][3], EAST | WEST);
    // add_wall(grid, grid[1][3], EAST | WEST);
    // add_wall(grid, grid[0][3], WEST);
    // add_wall(grid, grid[3][2], WEST | SOUTH);
    // add_wall(grid, grid[2][2], WEST);
    // add_wall(grid, grid[1][2], WEST);
    // add_wall(grid, grid[0][3], WEST);
    // add_wall(grid, grid[0][1], WEST);
    // add_wall(grid, grid[1][1], WEST);
    // add_wall(grid, grid[2][1], WEST);
    // add_wall(grid, grid[3][1], WEST);
    
    bool enabled = false;

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(10000));
        if (!enabled)
        {
            enabled = true;
            print_grid(car->grid);
            discover_map(car->grid, car->position);
            // struct LinkedList *path = compute_path(car->grid, car->grid[4][4], car->grid[0][0]);
            // if (path != NULL)
            // {
            //     print_ll(path);
            // }
            // free(path);
        }
    }
}

struct Node* lowest_common_ancestor(struct LinkedList* path, struct Node* node){

    if(node == path->head){
        return node;
    }

    struct Node* lca_node = lowest_common_ancestor(path, node->parent);

    if(lca_node != node->parent){
        insertAtHead(path, node);
        return lca_node;
    }

    else if(in_ll(path, node)){
        removeNode(path, lca_node);
        return node;
    }

    else{
        insertAtHead(path, node);
        return lca_node;
    }
    

}

struct LinkedList* generate_path_to_node(struct Node* start_node, struct Node* end_node){
    struct LinkedList* path = (struct LinkedList*)malloc(sizeof(struct LinkedList));
    ll_init(path);
    struct Node* curr_node = end_node;

    while (curr_node->parent != NULL && curr_node != start_node){
        insertAtHead(path, curr_node);
        curr_node = curr_node->parent;
    }

    if(curr_node != start_node){
        insertAtHead(path, curr_node);
        struct Node* lca_node = lowest_common_ancestor(path, start_node);
    }
    else{
        insertAtHead(path, start_node);
    }

    return path;
}

void visit_node(struct Node** car_position, struct Node* node_to_visit){
    struct LinkedList* path_to_node = generate_path_to_node(*car_position, node_to_visit);
    struct Node* starting_node = remove_at_head(path_to_node);

    while(!isEmpty(path_to_node)){
        struct Node* next_node = remove_at_head(path_to_node);
        printf("Goal: (%d,%d), traversing from (%d,%d) to (%d,%d)\n",node_to_visit->location.x, node_to_visit->location.y, (*car_position)->location.x, (*car_position)->location.y, next_node->location.x, next_node->location.y);
        *car_position = next_node;
    }

    free(path_to_node);
}

void discover_map(struct Node* grid[MAX_ROW][MAX_COL], struct Node** car_position){
    struct LinkedList* stack = (struct LinkedList*)malloc(sizeof(struct LinkedList));
    ll_init(stack);

    insertAtHead(stack, grid[START_NODE_X][START_NODE_Y]);
    
    while(!isEmpty(stack)){
        //print_ll(stack);
        struct Node* node_to_visit = remove_at_head(stack);
        node_to_visit->marked = true;

        visit_node(car_position, node_to_visit);
        // TODO: Check side walls, then turn 45 degrees and check front walls. Add walls using add_wall()

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
                    struct Node* neighbour_node = grid[pos_x][pos_y];
                    if(in_ll(stack, neighbour_node)){
                        removeNode(stack, neighbour_node);
                    }
                    //printf("neighbour node: (%d,%d)\n", neighbour_node->location.x, neighbour_node->location.y);
                    neighbour_node->parent = node_to_visit;
                    insertAtHead(stack, neighbour_node);
                }

                node_to_visit->is_walled &= ~mask;
            }
        }
    }

    printf("Successfully mapped the maze.\n");
    free(stack);
}