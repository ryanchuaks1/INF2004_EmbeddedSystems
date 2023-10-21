/*
 * Author: Leung Wei Jun
 * Description: TODO: Add description
 */

#ifndef GRID_H
#define GRID_H

#include "common.h"
#include "node.h"

#define MAX_ROW 5
#define MAX_COL 5
#define WALL_MASK 0x0F

#define START_NODE 24
#define END_NODE 0

void grid_init(struct Node* grid[MAX_ROW][MAX_COL]){
    for(uint8_t i = 0; i < MAX_ROW; i++){
        for(uint8_t j = 0; j < MAX_COL; j++){
            grid[i][j] = (struct Node*)malloc(sizeof(struct Node));

            node_init(grid[i][j]);

            grid[i][j]->location.x = i;
            grid[i][j]->location.y = j;

            if((i * MAX_ROW + j != START_NODE) && (i * MAX_ROW + j != END_NODE)){
                grid[i][j]->is_walled |= ((i == 0) ? NORTH : 0) | ((j == 0) ? WEST : 0) | ((i == MAX_ROW-1) ? SOUTH : 0) | ((j == MAX_COL-1) ? EAST : 0);
            }
        }
    }
}

void print_grid(struct Node* grid[MAX_ROW][MAX_COL]){
    for(uint8_t i = 0; i < MAX_ROW; i++){
        for(uint8_t j = 0; j < MAX_COL; j++){
            // printf("%c,%c,%c,%c]", (grid[i][j]->is_walled & NORTH) ? '^' : '.', (grid[i][j]->is_walled & SOUTH) ? 'V' : '.', (grid[i][j]->is_walled & EAST) ? '<' : '.', (grid[i][j]->is_walled & WEST) ? '>' : '.');
            printf("%s", (grid[i][j]->is_walled & NORTH) ? "----" : "    ");
        }
        printf("\n");
        for(uint8_t j = 0; j < MAX_COL; j++){
            printf("%c%.2d%c", (grid[i][j]->is_walled & WEST) ? '|' : ' ', (grid[i][j]->location.x * MAX_COL + grid[i][j]->location.y), (grid[i][j]->is_walled & EAST) ? '|' : ' ');
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

void add_wall(struct Node* grid[MAX_ROW][MAX_COL], struct Node* node, uint8_t wall_direction){
    node->is_walled |= wall_direction;

    for(uint8_t i = 0; i < 4; i++)
    {
        uint8_t mask = (1 << i);

        if((wall_direction & mask) > 0){
            uint8_t pos_x = node->location.x;
            uint8_t pos_y = node->location.y;

            if (mask & (NORTH | SOUTH))
            {
                pos_x += (mask % 2) ? -1 : 1;
                if(pos_x < MAX_ROW){
                    grid[pos_x][pos_y]->is_walled |= (~mask & (NORTH | SOUTH));
                }
            }
            else if(mask & (EAST | WEST))
            {
                pos_y += ((mask / 4) % 2) ? 1 : -1;
                if(pos_y < MAX_COL){
                    grid[pos_x][pos_y]->is_walled |= (~mask & (EAST | WEST));
                }
            }

            wall_direction &= ~mask;
        }
    }
}

#endif


