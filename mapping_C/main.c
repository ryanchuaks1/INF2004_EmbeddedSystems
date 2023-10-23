#include "common.h"
#include "grid.h"
#include "pathfinding.h"
#include "mapping.h"

int main(){
    struct Node* grid[MAX_ROW][MAX_COL];
    grid_init(grid);

    struct Node** car_position = (struct Node**)malloc(sizeof(struct Node*));
    *car_position = grid[START_NODE_X][START_NODE_Y];

    add_wall(grid, grid[1][1], NORTH | EAST | WEST);
    add_wall(grid, grid[2][2], NORTH | SOUTH | WEST);
    add_wall(grid, grid[3][3], NORTH | WEST);
    add_wall(grid, grid[0][1], SOUTH | EAST | WEST);

    print_grid(grid);

    // struct LinkedList* test = malloc(sizeof(struct LinkedList));
    // ll_init(test);
    // for(int i = 0; i<5; i++){
    //     grid[i][i]->f_cost = i+1;
    //     insertAtHead(test, grid[i][i]);
    // }
    // sort_f_cost(test);
    // print_ll(test);

    // struct LinkedList* path = compute_path(grid, grid[4][4], grid[0][0]);
    // if(path != NULL){
    //     print_ll(path);
    // }
    // free(path);

    discover_map(grid, car_position);

}