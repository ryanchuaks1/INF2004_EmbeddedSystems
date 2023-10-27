/*
 * Author: Ryan Chua
 * Description: Main file for the project
 */

#include "../include/main.h"

void print_text(const char *str)
{
    printf("%s", str);
}

void check_walls()
{
    if (ir_sensor_read(FRONT))
    {
        printf("Front\n");
        // TODO: stop motors
    }
    if (ir_sensor_read(LEFT))
    {
        printf("Left\n");
        // TODO: turn right
    }
    if (ir_sensor_read(RIGHT))
    {
        printf("Right\n");
        // TODO: turn left
    }
}

void test_rtos(void *pvParameters)
{
    while (1)
    {
        printf("Hello, world!\n");
        vTaskDelay(1000);
    }
}

void map_test(){
    struct Node* grid[MAX_ROW][MAX_COL];
    grid_init(grid);

    struct Node** car_position = (struct Node**)malloc(sizeof(struct Node*));
    *car_position = grid[START_NODE_X][START_NODE_Y];

    add_wall(grid, grid[3][3], SOUTH | EAST);
    add_wall(grid, grid[2][3], EAST | WEST);
    add_wall(grid, grid[1][3], EAST | WEST);
    add_wall(grid, grid[0][3], WEST);
    add_wall(grid, grid[3][2], WEST | SOUTH);
    add_wall(grid, grid[2][2], WEST);
    add_wall(grid, grid[1][2], WEST);
    add_wall(grid, grid[0][3], WEST);
    add_wall(grid, grid[0][1], WEST);
    add_wall(grid, grid[1][1], WEST);
    add_wall(grid, grid[2][1], WEST);
    add_wall(grid, grid[3][1], WEST);


    bool enabled = false;

    while(true){
        vTaskDelay(pdMS_TO_TICKS(10000));
        if(!enabled){
            enabled = true;
            print_grid(grid);
            discover_map(grid, car_position);
            struct LinkedList* path = compute_path(grid, grid[4][4], grid[0][0]);
            if(path != NULL){
                print_ll(path);
            }
            free(path);
        }
    }
}
}

void test_barcode(void *pvParameters)
{
    while (1)
    {
    }
    // check_if_wall();
}

int main()
{
    stdio_init_all();
    barcode_init();

    //xTaskCreate(test_barcode, "test_barcode", 1024, NULL, 1, NULL);
    xTaskCreate(map_test, "map_test", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();

    while (1)
    {
    };
}
