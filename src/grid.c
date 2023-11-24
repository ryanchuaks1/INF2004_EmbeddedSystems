#include "../include/grid.h"

/*
 * grid_init()
 * ---------------------
 * Purpose: After memory is allocated to the grid, initializes it by initializing every node in the grid as well
 * Reason: This is because each grid only stores the address to the node, hence malloc() for the grid does not allocate memory to the struct itself.
 * Arguments: Map grid
 * Returns: void
 */
void grid_init(struct Node* grid[MAX_ROW][MAX_COL]){
    for(uint8_t i = 0; i < MAX_ROW; i++){
        for(uint8_t j = 0; j < MAX_COL; j++){
            grid[i][j] = (struct Node*)malloc(sizeof(struct Node));

            node_init(grid[i][j]);

            grid[i][j]->location.x = i;
            grid[i][j]->location.y = j;
            
            // wall off the north/south walls of the nodes at the border of the grid (if they're not the start or the end node)
            if( ! ( ( (i == START_NODE_X) && (j == START_NODE_Y) ) || ( (i == END_NODE_X) && (j == END_NODE_Y) ) ) ){
                grid[i][j]->is_walled |= ((i == 0) ? NORTH : 0) | ((i == MAX_ROW-1) ? SOUTH : 0);
            }

            // wall off the east/west walls of the nodes at the border of the grid
            grid[i][j]->is_walled |= ((j == 0) ? WEST : 0) | ((j == MAX_COL-1) ? EAST : 0);
        }
    }
}

void print_grid(struct Node* grid[MAX_ROW][MAX_COL]){
    for(uint8_t i = 0; i < MAX_ROW; i++){
        for(uint8_t j = 0; j < MAX_COL; j++){
            printf("%s", (grid[i][j]->is_walled & NORTH) ? "----" : "    ");
        }
        printf("\n");
        for(uint8_t j = 0; j < MAX_COL; j++){
            printf("%c%d%d%c", (grid[i][j]->is_walled & WEST) ? '|' : ' ', grid[i][j]->location.x, grid[i][j]->location.y, (grid[i][j]->is_walled & EAST) ? '|' : ' ');
        }
        printf("\n");
        if(i == MAX_ROW-1){
            for(uint8_t j = 0; j < MAX_COL; j++){
                printf("%s", (grid[i][j]->is_walled & SOUTH) ? "----" : "    ");
            }
            printf("\n");
        }

    }
}

/*
 * add_wall()
 * ---------------------
 * Purpose: This function is called when a wall needs to be added on the node. If you add a west wall on the node, then the neighbouring left node's east wall also needs to be added.
 * Argument: Map grid, the node to add the wall on, which wall to add (North, South, East, West wall)
 * Returns: void
 */
void add_wall(struct Node* grid[MAX_ROW][MAX_COL], struct Node* node, uint8_t wall_direction){
    node->is_walled |= wall_direction;

    for(uint8_t i = 0; i < 4; i++)
    {
        uint8_t mask = (1 << i);

        //if there's a wall, then we need to add a wall on our neighbour also
        if((wall_direction & mask) > 0){
            uint8_t pos_x = node->location.x;
            uint8_t pos_y = node->location.y;

            if (mask & (NORTH | SOUTH))
            {
                pos_x += ((mask >> 2) % 2) ? 1 : -1;

                // because we're using unsigned integer, if the coordinates becomes < 0, it'll default to 255. Hence, it wouldn't pass the if-clause below.
                if(pos_x < MAX_ROW){
                    grid[pos_x][pos_y]->is_walled |= (~mask & (NORTH | SOUTH));
                }
            }
            else if(mask & (EAST | WEST))
            {
                pos_y += (mask % 2) ? -1 : 1;
                if(pos_y < MAX_COL){
                    grid[pos_x][pos_y]->is_walled |= (~mask & (EAST | WEST));
                }
            }

            wall_direction &= ~mask;
        }
    }
}