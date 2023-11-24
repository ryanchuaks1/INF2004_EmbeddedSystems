/*
 * Author: Wei Jun
 * Description: TODO: Add description
 */

#include "../include/mapping.h"

/*
 * mapping_task()
 * ---------------------
 * Purpose: Waits for the mapping algorithm to finish discovering the map, then use A* pathfinding to find the shortest path
 * Arguments: car struct
 * Returns: void
 */
void mapping_task(void* params)
{
    struct Car* car = (struct Car*)params;
    
    bool enabled = false;

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(10000));
        if (!enabled)
        {
            enabled = true;
            print_grid(car->grid);
            discover_map(car->grid, car->position);

            xMessageBufferSend(
                *(car->components[MAPPING]->buffer),
                (void*)&enabled,
                sizeof(enabled),
                portMAX_DELAY
            );
        }
    }
}

/*
 * lowest_common_ancestor()
 * ---------------------
 * Purpose: Recursive function, aims to find the intersecting node between 2 paths that lead to the entrance node
 * 1. To answer what the common node from the path to our destination node is, we need to check the LCA of the parent of the dest node
 * 2. This recursion goes all the way back to the entrance node, and returns the entrance node as that node is confirm the common node between the 2 paths.
 * 3. Then check if my current node is also in the path, if it is, then we can remove the entrance node (so we're not the lowest common ancestor), return myself to the parent function call
 * 4. Repeat step 3 until we reach a point where im not in the path, means that the lca_node is the last common node. So from there, we just keep adding all our parents back.
 * Arguments: The path computed (which leads all the way back to the entrance node), and the node that you wish to visit. Hence, it is to find the intersecting node which can connect these 2 path together (without resorting to travelling back to the entrance node)
 * Returns: The common node
 */
struct Node* lowest_common_ancestor(struct LinkedList* path, struct Node* node){

    if(node == path->head){
        return node;
    }

    struct Node* lca_node = lowest_common_ancestor(path, node->parent);

    //if i already found the LCA node, then i just continue adding myself into the linkedlist, and pass the lca to the parent function call.
    if(lca_node != node->parent){
        insertAtHead(path, node);
        return lca_node;
    }
    
    // if im also part of the path, then we can remove my parent.
    else if(in_ll(path, node)){
        removeNode(path, lca_node);
        return node;
    }

    // if i'm not in the linked list (means that this lca_node is the last common node intersecting both paths), return this lca_node so the 1st condition will activate from now on for any subsequent nodes
    else{
        insertAtHead(path, node);
        return lca_node;
    }
    

}

/*
 * generate_path_to_node()
 * ---------------------
 * Purpose: Generates a path from the starting node, to the destination node.
 * 1. Start from the destination node, add it to the linkedlist, then backtrack to its parent.
 * 2. Add the parent to the linked list, then backtrack to its parent again.
 * 3. Repeat 2 until the parent is a NULL refernce OR currnode is already the start node
 * 4.1. If the currnode is the start node, then just add it to the linkedlist and return
 * 4.2. However, if my parent's node is NULL and its not my start node, means that I've reached the entrance node (all nodes parent to this)
 * 5. From 4.2, we need to find the lowest common node that intersects both the node (so we don't have to travel all the way back to the start node then travel back up).
 * 6. This is essentially seeing where both paths intersect.
 * Arguments: Start and end node
 * Returns: A path from the start node to the end node.
 */
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

/*
 * visit_node()
 * ---------------------
 * Purpose: From the car's current position, generate a path to the next node we need to visit.
 * After which, visit the nodes 1 by 1, until the destination is reached.
 * Arguments: Current car's position, and the node you wish to visit
 * Returns: void, cause this function forces the car to move
 */
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

/*
 * discover_map()
 * ---------------------
 * Purpose: DFS algorithm, does the following,
 * 1. Add the starting node into the stack
 * 2. Pop the stack, visit that node, then add its neighbours to the stack as well (neighbours are not added if there's a wall)
 * 3. Repeat 2 until the stack is empty.
 * Arguments: Map grid, and the current car's position.
 * Returns: void
 */
void discover_map(struct Node* grid[MAX_ROW][MAX_COL], struct Node** car_position){
    struct LinkedList* stack = (struct LinkedList*)malloc(sizeof(struct LinkedList));
    ll_init(stack);

    insertAtHead(stack, grid[START_NODE_X][START_NODE_Y]);
    
    while(!isEmpty(stack)){

        struct Node* node_to_visit = remove_at_head(stack);
        node_to_visit->marked = true;

        visit_node(car_position, node_to_visit);

        // TODO: Check neighbours for walls, then send the wall data to the mapping message buffer.
        
        // uint8_t walls_detected;
        // xMessageBufferReceive(
        //     *(global_car->components[MAPPING]->buffer),
        //     (void*)&walls_detected,
        //     sizeof(walls_detected),
        //     portMAX_DELAY
        // );
        // add_wall(grid, node_to_visit, walls_detected);

        // For each node, check if each wall is blocked (N, S, E, W)
        for(uint8_t i = 0; i < 4; i++)
        {
            uint8_t mask = (1 << i);

            // if the wall we're checking is not blocked (blocked = 1, not blocked = 0)
            if((node_to_visit->is_walled & mask) == 0){
                uint8_t pos_x = node_to_visit->location.x;
                uint8_t pos_y = node_to_visit->location.y;

                // if its a North or South wall, then the x-coordinate will decrease/increase
                if (mask & (NORTH | SOUTH))
                {
                    pos_x += ((mask >> 2) % 2) ? 1 : -1;
                }
                // if its a East or West wall, then the y-coordinate will decrease/increase
                else if(mask & (EAST | WEST))
                {
                    pos_y += (mask % 2) ? -1 : 1;
                }

                // If the node is not out of the grid's bounds, and has not yet been visited, then add it to the stack.
                if(pos_x < MAX_ROW && pos_y < MAX_COL && !(grid[pos_x][pos_y]->marked)){
                    struct Node* neighbour_node = grid[pos_x][pos_y];

                    // if the node is already in the stack, then remove it first.
                    if(in_ll(stack, neighbour_node)){
                        removeNode(stack, neighbour_node);
                    }

                    // parent this node to our current node, since to get to this node, you need to come from my current node
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