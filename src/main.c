/*
 * Author: Ryan Chua
 * Description: Main file for the project
 */

#include "../include/main.h"

void enter(struct Car* car){
    switch(car->state){
        case IDLE:
            break;
        case TRANSIT:
            break;
        case ADJUST:
            break;
        case BARCODE:
            break;
    }
}

void execute(struct Car* car){
    switch(car->state){
        case IDLE:
            break;
        case TRANSIT:
            break;
        case ADJUST:
            break;
        case BARCODE:
            break;
    }
}

void exit(struct Car* car){
    switch(car->state){
        case IDLE:
            break;
        case TRANSIT:
            break;
        case ADJUST:
            break;
        case BARCODE:
            break;
    }
}

void change_state(struct Car* car, enum PID_STATE next_state){
    if(car->state != NULL){
        exit(car);
    }

    car->state = next_state;

    enter(car);
}

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

void main_task(void* params)
{
    struct Car* car = (struct Car*)params;

    while(true){
        execute(car);
    }
}

int main()
{
    struct Car* car = (struct Car*)malloc(sizeof(struct Car));
    car->state = NULL;

    change_state(car, IDLE);

    stdio_init_all();
    barcode_init();   // Initialise barcode scanner,
    ir_sensor_init(); // Initialise IR sensors
    l298n_speed_pwm_setup(); // Initialise PWM for L298N

    xTaskCreate(main_task, "main_task", configMINIMAL_STACK_SIZE, car, 3, NULL);
    xTaskCreate(motor_task, "motor_task", configMINIMAL_STACK_SIZE, car, 2, NULL);
    
    vTaskStartScheduler();

    while (1)
    {
    };
}

// fdasd