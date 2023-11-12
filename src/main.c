/*
 * Author: Ryan Chua
 * Description: Main file for the project
 */

#include "../include/main.h"

void state_enter(struct Car* car){
    switch(*(car->state)){
        case IDLE:
            break;
        case TRANSIT:
            break;
        case ADJUST:
            break;
        case SCANNING:
            break;
    }
}

void state_execute(struct Car* car){
    switch(*(car->state)){
        case IDLE:
            break;
        case TRANSIT:
            break;
        case ADJUST:
            break;
        case SCANNING:
            break;
    }
}

void state_exit(struct Car* car){
    switch(*(car->state)){
        case IDLE:
            break;
        case TRANSIT:
            break;
        case ADJUST:
            break;
        case SCANNING:
            break;
    }
}

void change_state(struct Car* car, enum PID_STATE next_state){
    if(car->state != NULL){
        state_exit(car);
    }
    else{
        car->state = (enum PID_STATE*)malloc(sizeof(enum PID_STATE));
        if(car->state == NULL){
            printf("Malloc failed. Terminating program...\n");
            exit(1);
        }
    }

    *(car->state) = next_state;

    state_enter(car);
}

void car_init(struct Car* car){
    if(car == NULL){
        printf("Malloc failed. Terminating program...\n");
        exit(1);
    }
    else{
        car->position = NULL;
        car->state = NULL;
        for(uint8_t i = 0; i < COMPONENTS_COUNT; i++){
            car->components[i] = NULL;
        }
    }
}

void components_init(struct Component* components[COMPONENTS_COUNT]){
    for(uint8_t i = 0; i < COMPONENTS_COUNT; i++){
        components[i] = (struct Component*)malloc(sizeof(struct Component));
        component_init(components[i]);
        *(components[i]->buffer) = xMessageBufferCreate(mbaTASK_MESSAGE_BUFFER_SIZE);
        components[i]->task_handler = (TaskHandle_t*)malloc(sizeof(TaskHandle_t));
        if(components[i]->task_handler == NULL){
            printf("Malloc failed. Terminating program...\n");
            exit(1);
        }
    }
}

void component_init(struct Component* component){
    if(component == NULL){
        printf("Malloc failed. Terminating program...\n");
        exit(1);
    }
    else{
        component->buffer = NULL;
        component->task_handler = NULL;
        component->flags = NULL;
    }
}

void main_task(void* params)
{
    struct Car* car = (struct Car*)params;
    change_state(car, IDLE);

    while(true){
        state_execute(car);
    }
}

int main()
{
    struct Car* car = (struct Car*)malloc(sizeof(struct Car));
    car_init(car);
    grid_init(car->grid);
    components_init(car->components);

    car->position = (struct Node**)malloc(sizeof(struct Node*));
    *(car->position) = car->grid[START_NODE_X][START_NODE_Y];

    stdio_init_all();
    barcode_init();   // Initialise barcode scanner,
    ir_sensor_init(); // Initialise IR sensors
    l298n_speed_pwm_setup(); // Initialise PWM for L298N

    xTaskCreate(mapping_task, "mapping_task", configMINIMAL_STACK_SIZE, car, MAPPING_TASK_PRIORITY, car->components[MAPPING]->task_handler);
    xTaskCreate(motor_task, "motor_task", configMINIMAL_STACK_SIZE, car, MOTOR_TASK_PRIORITY, car->components[MOTOR]->task_handler);
    xTaskCreate(wheel_encoder_task, "wheel_encoder_task", configMINIMAL_STACK_SIZE, car, WHEEL_ENCODER_TASK_PRIORITY, car->components[WHEEL_ENCODER]->task_handler);    
    xTaskCreate(barcode_task, "barcode_task", configMINIMAL_STACK_SIZE, car, BARCODE_TASK_PRIORITY, car->components[BARCODE]->task_handler);
    xTaskCreate(ultrasonic_task, "ultrasonic_task", configMINIMAL_STACK_SIZE, car, ULTRASONIC_TASK_PRIORITY, car->components[ULTRASONIC]->task_handler);
    xTaskCreate(infrared_task, "infrared_task", configMINIMAL_STACK_SIZE, car, INFRARED_TASK_PRIORITY, car->components[INFRARED]->task_handler);
    xTaskCreate(magnetometer_task, "magnetometer_task", configMINIMAL_STACK_SIZE, car, MAGNETOMETER_TASK_PRIORITY, car->components[MAGNETOMETER]->task_handler);
    vTaskStartScheduler();

    while (1);
}