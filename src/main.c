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
    size_t xBytesSent;
    size_t xBytesReceived;

    char* opcode = "";

    uint16_t starting_left_count;
    uint16_t starting_right_count;
    
    static uint16_t duration_ms = 10000;
    static enum DIRECTION direction = FORWARD;

    switch(*(car->state)){
        case IDLE:
            printf("Main Menu. 1 = Change duty cycle, 2 = Calibrate wheels, 3 = Start car\n");
            char input = getchar();
            switch(input){
                case '1':
                    printf("Enter a number from 0 to 9, where 0 = 10%%, 9 = 100%%\n");
                    char duty_cycle_input = getchar();
                    car->duty_cycle = (float)atoi(&duty_cycle_input) / 10;
                    break;
                case '2':
                    starting_left_count = left_rising_edge_count;
                    starting_right_count = right_rising_edge_count;
                    
                    uint32_t start_time_us = time_us_32();

                    set_forward();
                    set_speed(car->duty_cycle, car->wheels_ratio);
                    vTaskDelay(pdMS_TO_TICKS(duration_ms));
                    set_stop();

                    car->wheels_ratio = (float)(left_rising_edge_count - starting_left_count) / (float)(right_rising_edge_count - starting_right_count);
                    printf("Left count: %d\n", (left_rising_edge_count - starting_left_count));
                    printf("Right count: %d\n", (right_rising_edge_count - starting_right_count));
                    printf("Calibration completed. Ratio is: %f\n", car->wheels_ratio);
                    break;
                case '3':
                    printf("Enter 0 to 9, where 0 = 1s, 9 = 10s\n");
                    char duration_input = getchar();
                    duration_ms = ((uint16_t)atoi(&duration_input) + 1) * 1000;

                    printf("Enter direction: F = Forward, B = Backward, L = Left, R = Right\n");
                    char direction_input = getchar();
                    switch(direction_input){
                        case 'F':
                            direction = FORWARD;
                            break;
                        case 'B':
                            direction = BACKWARD;
                            break;
                        case 'L':
                            direction = LEFT;
                            break;
                        case 'R':
                            direction = RIGHT;
                            break;
                    }

                    change_state(car, TRANSIT);
                    break;
                default:
                    printf("Invalid input\n");
                    break;
            }

            vTaskDelay(pdMS_TO_TICKS(1000));
            break;
        case TRANSIT:
            starting_left_count = left_rising_edge_count;
            starting_right_count = right_rising_edge_count;

            uint32_t start_time_us = time_us_32();
            set_direction(direction);
            set_speed(car->duty_cycle, car->wheels_ratio);
            
            xBytesSent = xMessageBufferSend(
                *(car->components[INFRARED]->buffer),
                (void*)&duration_ms,
                sizeof(duration_ms),
                0
            );

            xMessageBufferReceive
            (
                *(car->main_buffer),
                (void*)&opcode,
                sizeof(opcode),
                pdMS_TO_TICKS(duration_ms)
            );

            if(strcmp(opcode, "IR_IRQ") == 0){
                duration_ms -= (time_us_32() - start_time_us) / 1000;
                change_state(car, SCANNING);
            }

            else if((float)(time_us_32() - start_time_us) / 1000 >= (float)duration_ms){
                printf("Finished...\n");
                change_state(car, IDLE);
            }

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
            set_stop();
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
        car->wheels_ratio = 1.0;
        car->duty_cycle = 0.0;
    }
}

void components_init(struct Component* components[COMPONENTS_COUNT]){
    for(uint8_t i = 0; i < COMPONENTS_COUNT; i++){
        components[i] = (struct Component*)malloc(sizeof(struct Component));
        component_init(components[i]);

        // switch(i){
        //     case BARCODE:
        //         components[i]->buffer = &barcode_buffer;
        //         break;
        //     case INFRARED:
        //         components[i]->buffer = &infrared_buffer;
        //         break;
        //     case MAGNETOMETER:
        //         components[i]->buffer = &magnetometer_buffer;
        //         break;
        //     case MAPPING:
        //         components[i]->buffer = &mapping_buffer;
        //         break;
        //     case MOTOR:
        //         components[i]->buffer = &test_buffer;
        //         break;
        //     case ULTRASONIC:
        //         components[i]->buffer = &ultrasonic_buffer;
        //         break;
        //     case WHEEL_ENCODER:
        //         components[i]->buffer = &wheel_encoder_buffer;
        //         break;
        // }

        // printf("Component: %d, Buffer address: %p\n", i, components[i]->buffer);

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
    printf("Message buffer address from main_task: %p\n", car->components[MOTOR]->buffer);
    while(true){
        state_execute(car);
    }
}

void vLaunch(struct Car* car){
    MessageBufferHandle_t barcode_buffer = xMessageBufferCreate(mbaTASK_MESSAGE_BUFFER_SIZE);
    MessageBufferHandle_t infrared_buffer = xMessageBufferCreate(mbaTASK_MESSAGE_BUFFER_SIZE);
    MessageBufferHandle_t magnetometer_buffer = xMessageBufferCreate(mbaTASK_MESSAGE_BUFFER_SIZE);
    MessageBufferHandle_t mapping_buffer = xMessageBufferCreate(mbaTASK_MESSAGE_BUFFER_SIZE);
    MessageBufferHandle_t motor_buffer = xMessageBufferCreate(mbaTASK_MESSAGE_BUFFER_SIZE);
    MessageBufferHandle_t ultrasonic_buffer = xMessageBufferCreate(mbaTASK_MESSAGE_BUFFER_SIZE);
    MessageBufferHandle_t wheel_encoder_buffer = xMessageBufferCreate(mbaTASK_MESSAGE_BUFFER_SIZE);
    MessageBufferHandle_t main_buffer = xMessageBufferCreate(mbaTASK_MESSAGE_BUFFER_SIZE);

    car->components[BARCODE]->buffer = &barcode_buffer;
    car->components[INFRARED]->buffer = &infrared_buffer;
    car->components[MAGNETOMETER]->buffer = &magnetometer_buffer;
    car->components[MAPPING]->buffer = &mapping_buffer;
    car->components[MOTOR]->buffer = &motor_buffer;
    car->components[ULTRASONIC]->buffer = &ultrasonic_buffer;
    car->components[WHEEL_ENCODER]->buffer = &wheel_encoder_buffer;
    car->main_buffer = &main_buffer;


    xTaskCreate(main_task, "main_task", configMINIMAL_STACK_SIZE, (void *)car, MAIN_TASK_PRIORITY, NULL);
    // xTaskCreate(mapping_task, "mapping_task", configMINIMAL_STACK_SIZE, car, MAPPING_TASK_PRIORITY, car->components[MAPPING]->task_handler);
    xTaskCreate(motor_task, "motor_task", configMINIMAL_STACK_SIZE, (void *)car, MOTOR_TASK_PRIORITY, car->components[MOTOR]->task_handler);
    xTaskCreate(wheel_encoder_task, "wheel_encoder_task", configMINIMAL_STACK_SIZE, (void *)car, WHEEL_ENCODER_TASK_PRIORITY, car->components[WHEEL_ENCODER]->task_handler);    
    // xTaskCreate(barcode_task, "barcode_task", configMINIMAL_STACK_SIZE, car, BARCODE_TASK_PRIORITY, car->components[BARCODE]->task_handler);
    // xTaskCreate(ultrasonic_task, "ultrasonic_task", configMINIMAL_STACK_SIZE, car, ULTRASONIC_TASK_PRIORITY, car->components[ULTRASONIC]->task_handler);
    // xTaskCreate(infrared_task, "infrared_task", configMINIMAL_STACK_SIZE, car, INFRARED_TASK_PRIORITY, car->components[INFRARED]->task_handler);
    // xTaskCreate(magnetometer_task, "magnetometer_task", configMINIMAL_STACK_SIZE, car, MAGNETOMETER_TASK_PRIORITY, car->components[MAGNETOMETER]->task_handler);
    vTaskStartScheduler();
}

int main()
{
    stdio_init_all();

    sleep_ms(5000);

    struct Car* car = (struct Car*)malloc(sizeof(struct Car));
    car_init(car);
    grid_init(car->grid);
    components_init(car->components);

    car->position = (struct Node**)malloc(sizeof(struct Node*));
    *(car->position) = car->grid[START_NODE_X][START_NODE_Y];

    vLaunch(car);
}