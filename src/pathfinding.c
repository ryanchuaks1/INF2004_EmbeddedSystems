#include "../include/pathfinding.h"

struct LinkedList* generate_path(struct Node* end_node, struct Node* start_node){
    struct LinkedList* path = (struct LinkedList*)malloc(sizeof(struct LinkedList));
    ll_init(path);
    struct Node* curr_node = end_node;

    while (curr_node != start_node){
        insertAtHead(path, curr_node);
        curr_node = curr_node->parent;
    }

    insertAtHead(path, start_node);

    return path;
}

// float calculate_f_cost(struct Node* curr_node){
//     return curr_node->g_cost + curr_node->h_cost;
// }

void sort_f_cost(struct LinkedList* ll){
    ll->head = merge_sort(ll->head);
    struct Node* curr_node = ll->head;
    while(curr_node->next != NULL){
        curr_node = curr_node->next;
    }
    ll->tail = curr_node;
}

uint8_t calculate_g_cost(struct Node* curr_node, struct Node* start_node){
    struct Node* node = curr_node;
    uint8_t g_cost = 0;

    while (node != start_node){
        g_cost += PATH_COST;
        node = node->parent;
    }

    return g_cost;
}

float calculate_h_cost(struct Node* curr_node, struct Node* end_node){
    int8_t delta_y = curr_node->location.y - end_node->location.y;
    int8_t delta_x = curr_node->location.x - end_node->location.x;

    return sqrt(pow(delta_x * PATH_COST, 2) + pow(delta_y * PATH_COST, 2));
}

struct LinkedList* compute_path(struct Node* grid[MAX_ROW][MAX_COL], struct Node* start_node, struct Node* end_node){
    struct LinkedList* open_list = (struct LinkedList*)malloc(sizeof(struct LinkedList));
    struct LinkedList* closed_list = (struct LinkedList*)malloc(sizeof(struct LinkedList));

    ll_init(open_list);
    ll_init(closed_list);

    insertAtTail(open_list, start_node);
    struct Node* curr_node = NULL;

    while (!isEmpty(open_list)){
        curr_node = open_list->head;
        removeNode(open_list, curr_node);

        if (curr_node == end_node){
            return generate_path(end_node, start_node);
        }

        for(uint8_t i = 0; i < 4; i++)
        {
            uint8_t mask = (1 << i);

            // if there's no wall, then this AND operation will result to 0
            if((curr_node->is_walled & mask) == 0){
                uint8_t pos_x = curr_node->location.x;
                uint8_t pos_y = curr_node->location.y;

                // check if the mask is currently servicing NORTH or SOUTH walls (which is mask = 8 or mask = 4)
                if (mask & (NORTH | SOUTH))
                {
                    // if our neighbour is NORTH of us ((8 / 4) % 2 = 0), then the x-coordinate - 1
                    // if our neighbour is SOUTH of us ((4 / 4) % 2 = 1), then the x-coordinate + 1
                    pos_x += ((mask >> 2) % 2) ? 1 : -1;

                }
                else if(mask & (EAST | WEST))
                {
                    // if our neighbour is EAST of us (2 % 2 = 0), then the y-coordinate + 1
                    // if our neighbour is WEST of us (1 % 2 = 1), then the y-coordinate - 1
                    pos_y += (mask % 2) ? -1 : 1;
                }

                if(pos_x < MAX_ROW && pos_y < MAX_COL){
                    struct Node* neighbour_node = grid[pos_x][pos_y];

                    if(in_ll(closed_list, neighbour_node)){
                        continue;
                    }

                    else if(in_ll(open_list, neighbour_node)){
                        if(curr_node->g_cost + PATH_COST < neighbour_node->g_cost){
                            neighbour_node->parent = curr_node;
                            neighbour_node->g_cost = calculate_g_cost(neighbour_node, start_node);
                            neighbour_node->f_cost = neighbour_node->g_cost + neighbour_node->h_cost;
                        }
                    }

                    else{
                        insertAtTail(open_list, neighbour_node);
                        neighbour_node->parent = curr_node;
                        neighbour_node->g_cost = calculate_g_cost(neighbour_node, start_node);
                        neighbour_node->h_cost = calculate_h_cost(neighbour_node, end_node);
                        neighbour_node->f_cost = neighbour_node->g_cost + neighbour_node->h_cost;
                    }
                }

                curr_node->is_walled &= ~mask;
            }
        }

        insertAtTail(closed_list, curr_node);
        sort_f_cost(open_list);
    }

    free(open_list);
    free(closed_list);
    return NULL;
}
