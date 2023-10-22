#include "common.h"
#include "grid.h"
#include "pathfinding.h"

int main(){
    struct Node* grid[MAX_ROW][MAX_COL];
    grid_init(grid);

    add_wall(grid, grid[1][1], NORTH | SOUTH | EAST | WEST);
    add_wall(grid, grid[2][2], NORTH | SOUTH | EAST | WEST);
    add_wall(grid, grid[3][3], NORTH | SOUTH | EAST | WEST);
    add_wall(grid, grid[0][1], NORTH | SOUTH | EAST | WEST);

    print_grid(grid);

    // struct LinkedList* test = malloc(sizeof(struct LinkedList));
    // ll_init(test);
    // for(int i = 0; i<5; i++){
    //     grid[i][i]->f_cost = i+1;
    //     insertAtHead(test, grid[i][i]);
    // }
    // sort_f_cost(test);
    // print_ll(test);
    struct LinkedList* path = compute_path(grid, grid[4][4], grid[0][0]);
    if(path != NULL){
        print_ll(path);
    }
    free(path);

}