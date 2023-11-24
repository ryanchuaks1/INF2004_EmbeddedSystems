/*
 * Author: Wei Jun
 * Description: A* Pathfinding related methods
 */

#include "../include/pathfinding.h"

/*
 * pathfinding_task()
 * ---------------------
 * Purpose: Waits for the mapping algorithm to finish discovering the map, then use A* pathfinding to find the shortest path
 * Arguments: car struct
 * Returns: void
 */
void pathfinding_task(void* params){
    struct Car* car = (struct Car*)params;
    
    bool enabled = false;
    vTaskDelay(pdMS_TO_TICKS(10000));

    while (true)
    {
        xMessageBufferReceive(
            *(car->components[MAPPING]->buffer),
            (void *)&enabled,
            sizeof(enabled),
            portMAX_DELAY
        );

        if (enabled)
        {
            struct LinkedList *path = compute_path(car->grid, car->grid[4][4], car->grid[0][0]);
            if (path != NULL)
            {
                print_ll(path);
            }
            free(path);
            enabled = false;
        }
    }
}

/*
 * generate_path()
 * ---------------------
 * Purpose: Only used when the A* Pathfinding has completed, and a path needs to be generated from the goal node back to the start node.
 * 1. Start from the end node, add itself to the linkedlist, then backtrack to its parent
 * 2. Add the parent into the linkedlist, then backtrack to the parent
 * 3. Repeat 2 until the parent is the start node
 * 4. A linkedlist containing the path is created.
 * Arguments: End node and Start node
 * Returns: Path from the start node to the end node (most efficient route)
 */
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
/*
 * sort_f_cost()
 * ---------------------
 * Purpose: Sorts the list by f-cost, uses merge_sort() to accomplish this.
 * 1. Linkedlist head will point to the sorted head node
 * 2. Linkedlist tail will point to the sorted tail node.
 * Arguments: Unsorted linkedlist
 * Returns: void, as the linkedlist will be sorted already.
 */
void sort_f_cost(struct LinkedList* ll){
    ll->head = merge_sort(ll->head);
    struct Node* curr_node = ll->head;
    while(curr_node->next != NULL){
        curr_node = curr_node->next;
    }
    ll->tail = curr_node;
}

/*
 * calculate_g_cost()
 * ---------------------
 * Purpose: Part of A* pathfinding parameter, it is the cost it takes to get to this node from the starting node. (cost to get here from previous node + previous node's g-cost)
 * 1. Keep adding the PATH_COST until we reach the starting node.
 * Arguments: current node & start node
 * Returns: Recalculated g-cost
 */
uint8_t calculate_g_cost(struct Node* curr_node, struct Node* start_node){
    struct Node* node = curr_node;
    uint8_t g_cost = 0;

    while (node != start_node){
        g_cost += PATH_COST;
        node = node->parent;
    }

    return g_cost;
}

/*
 * calculate_h_cost()
 * ---------------------
 * Purpose: Part of A* Pathfinding parameter, the h-cost is the distance from the current node to the goal node. Use pythagoras theorem as an estimate.
 * Arguments: current node & end node
 * Returns: Computed h-cost
 */
float calculate_h_cost(struct Node* curr_node, struct Node* end_node){
    int8_t delta_y = curr_node->location.y - end_node->location.y;
    int8_t delta_x = curr_node->location.x - end_node->location.x;

    return sqrt(pow(delta_x * PATH_COST, 2) + pow(delta_y * PATH_COST, 2));
}

/*
 * compute_path()
 * ---------------------
 * Purpose: A* pathfinding, computes the shortest path to the goal node using f-cost as a deciding factor on which node to visit first (in order to get the fastest computational speed to the goal node)
 * 1. 2 lists are maintained, open list and closed list. Open List = All nodes that aren't visited and checked yet. Closed List = All nodes that are visited and checked already.
 * 2. Add the neighbours of the starting node to the open list (all those neighbouring squares that are not walled off are your neighbours)
 * 3.1 Compute their f,g,h cost of the neighbouring nodes, set those nodes parent to be this node
 * 3.2 If the node already exist in the open list (neighbour of some other node that you visited already), then check if the cost to get to this node via my path is faster than the original path (re-compute g-cost).
 * 3.3 If the neighbouring node is in the closed list, then ignore it as we've already checked that node.
 * If it is, then re-parent that neighbouring node to me instead.
 * 4. Once all neighbouring nodes are settle, add this node to the closed list, then sort the entire open list by f-cost. Hence, the lowest f-cost node will be visited next (dequeued)
 * 5. This ensures that the node which provides us with the most value is always visited first (takes the least amount of path cost to get there + closest distance to the goal = lowest f-cost)
 * Arguments: The map's grid, starting node and goal node.
 * Returns: Shortest path from the start node to the goal node
 */
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
