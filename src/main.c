/*
 * Author: Ryan Chua
 * Description: Main file for the project
 */

#include "../include/main.h"

void print_text(const char *str)
{
    printf("%s", str);
}

void map_test()
{
    struct Node *grid[MAX_ROW][MAX_COL];
    grid_init(grid);

    struct Node **car_position = (struct Node **)malloc(sizeof(struct Node *));
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

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(10000));
        if (!enabled)
        {
            enabled = true;
            print_grid(grid);
            discover_map(grid, car_position);
            struct LinkedList *path = compute_path(grid, grid[4][4], grid[0][0]);
            if (path != NULL)
            {
                print_ll(path);
            }
            free(path);
        }
    }
}

void mapping()
{
    int timePassed = 0;
    int confidence_count_left_wall = 0;
    int confidence_count_right_wall = 0;
    while (true)
    {
        movement(NORTH);
        vTaskDelay(pdMS_TO_TICKS(100));
        timePassed += 100;
        
        ir_sensor_read(LEFT);
        ir_sensor_read(RIGHT);
        ir_sensor_read(FRONT);
        // ultrasonic_read();

        if (timePassed >= 1000)
        {
            timePassed = 0; // Reset timePassed
            // add_walls_to_map()
            // 
        }
    }
}

void movement(uint8_t direction)
{
    switch (direction)
    {
    case NORTH:
        set_forward();
        set_speed(0.5);

    default:
        break;
    }
}

void big_brain()
{
    switch (state)
    {
    case IDLE:
    {
    }
    case MOVING:
    {
        xTaskCreate(mapping, "mapping", 1024, NULL, 1, NULL);
    }
    case ADJUSTING:
    {
    }
    case BARCODE:
    {
    }
    }
}

int main()
{
    stdio_init_all();
    barcode_init();   // Initialise barcode scanner,
    ir_sensor_init(); // Initialise IR sensors
    l298n_speed_pwm_setup(); // Initialise PWM for L298N

    xTaskCreate(big_brain, "big_brain", 1024, NULL, 0, NULL);
    vTaskStartScheduler();

    while (1)
    {
    };
}

// fdasd