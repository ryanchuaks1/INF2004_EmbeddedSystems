/*
 * Author: Ryan Chua
 * Description: Main file for the project
 */

#include "../include/main.h"

struct Car *global_car;

void state_enter(struct Car *car)
{
    switch (*(car->state))
    {
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

void reset_msg_from_cgi()
{
    message_cgi = CGI_NULL_MESSAGE;
    message_cgi_value = CGI_NULL_VALUE;
    message_cgi_direction = CGI_NULL_DIRECTION;
}

void state_execute(struct Car *car)
{
    size_t xBytesSent;
    size_t xBytesReceived;

    uint8_t opcode = 10;

    uint16_t starting_left_count;
    uint16_t starting_right_count;

    static uint16_t duration_ms = 10000;
    static enum DIRECTION direction = FORWARD;

    uint8_t turn_interrupt_count = 10;

    switch (*(car->state))
    {
    case IDLE:
        printf("Main Menu. 1 = Change duty cycle, 2 = Calibrate wheels, 3 = Start car\n");
        switch (message_cgi)
        {
        case CGI_MAIN_MENU:
        {
            printf("Enter a number from 0 to 9, where 0 = 10%%, 9 = 100%%\n");
            if (message_cgi_value < CGI_NULL_VALUE)
            {
                printf("Changing duty cycle to %d%%\n", message_cgi_value * 10);
                car->duty_cycle = (float)(message_cgi_value) / 10; // Convert character to integer value and calculate duty cycle
                reset_msg_from_cgi();
                break;
            }
            break;
        }
        case CGI_CALIB_WHEELS:
        {
            printf("Calibrating wheels...\n");
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
            reset_msg_from_cgi();
            break;
        }
        case CGI_START_CAR:
        {
            printf("Enter direction: F = Forward, B = Backward, L = Left, R = Right\n");
            printf("Direction chosen is %d\n", CGI_NULL_DIRECTION);

            if (message_cgi_direction < CGI_NULL_DIRECTION)
            {
                switch (message_cgi_direction)
                {
                case CGI_FORWARD:
                    printf("Enter 0 to 9, where 0 = 1s, 9 = 10s\n");
                    if (message_cgi_value < CGI_NULL_VALUE)
                    {
                        duration_ms = (uint16_t)(message_cgi_value) * 1000;
                        direction = FORWARD;
                        change_state(car, TRANSIT);
                        reset_msg_from_cgi();
                    }
                    break;
                case CGI_BACKWARD:
                    direction = BACKWARD;
                    change_state(car, ADJUST);
                    reset_msg_from_cgi();
                    break;
                case CGI_LEFT:
                    direction = LEFT;
                    change_state(car, ADJUST);
                    reset_msg_from_cgi();
                    break;
                case CGI_RIGHT:
                    direction = RIGHT;
                    change_state(car, ADJUST);
                    reset_msg_from_cgi();
                    break;
                }
            }
            break;
        }
        default:
            printf("Current input is %d\n", message_cgi);
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(1500)); // Poll every 1.5 seconds
        break;
    case TRANSIT:
        xBytesSent = xMessageBufferSend(
            *(car->components[INFRARED]->buffer),
            (void *)&duration_ms,
            sizeof(duration_ms),
            0);
        xBytesSent = xMessageBufferSend(
            *(car->components[ULTRASONIC]->buffer),
            (void *)&duration_ms,
            sizeof(duration_ms),
            0);
        uint32_t start_time_us = time_us_32();
        set_direction(direction);
        set_speed(car->duty_cycle, car->wheels_ratio);
        xMessageBufferReceive(
            *(car->main_buffer),
            (void *)&opcode,
            sizeof(opcode),
            pdMS_TO_TICKS(duration_ms));
        printf("Received: %f\n", (float)(time_us_32() - start_time_us) / 1000);
        if (opcode == BARCODE)
        {
            duration_ms -= (time_us_32() - start_time_us) / 1000;
            direction = BACKWARD;
            change_state(car, SCANNING);
        }
        else if (opcode == INFRARED)
        {
            set_stop();
            set_speed(car->duty_cycle, car->wheels_ratio);
            turn_with_interrupts(car, BACKWARD, turn_interrupt_count);
            turn_with_interrupts(car, RIGHT, turn_interrupt_count);
            turn_with_interrupts(car, FORWARD, turn_interrupt_count);
            change_state(car, IDLE);
            // direction = RIGHT;
            //  change_state(car, ADJUST);
        }
        else if (opcode == ULTRASONIC)
        {
            // duration_ms -= (time_us_32() - start_time_us) / 1000;
            printf("Activate Ultraconic State....");
            change_state(car, OBSTACLE);
        }
        else if ((float)(time_us_32() - start_time_us) / 1000 >= (float)duration_ms)
        {
            printf("Finished...\n");
            change_state(car, IDLE);
        }

        // else{
        //     printf("Error occured, received: %d\n", opcode);
        // }

        break;
    case ADJUST:
        starting_left_count = left_rising_edge_count;
        starting_right_count = right_rising_edge_count;
        set_direction(direction);
        set_speed(car->duty_cycle, car->wheels_ratio);

        while (((left_rising_edge_count - starting_left_count) < turn_interrupt_count) &&
               ((right_rising_edge_count - starting_right_count) < turn_interrupt_count))
        {
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        set_stop();
        change_state(car, IDLE);
        break;
    case SCANNING:
        set_direction(BACKWARD);
        set_speed(car->duty_cycle, car->wheels_ratio);
        vTaskDelay(pdMS_TO_TICKS(100));
        set_stop();
        opcode = BARCODE;
        xBytesSent = xMessageBufferSend(
            *(car->components[BARCODE]->buffer),
            (void *)&opcode,
            sizeof(opcode),
            portMAX_DELAY);

        set_speed(car->duty_cycle, car->wheels_ratio);
        set_direction(FORWARD);

        xMessageBufferReceive(
            *(car->main_buffer),
            (void *)&opcode,
            sizeof(opcode),
            portMAX_DELAY);

        if (opcode == BARCODE)
        {
            set_stop();
            change_state(car, IDLE);
        }
        break;
    case OBSTACLE:
        printf("Inside Obstacle state\n");
        // set_stop();
        // set_speed(car->duty_cycle, car->wheels_ratio);
        // turn_with_interrupts(car, LEFT, U_TURN_INTERRUPT);
        // set_stop();
        set_speed(car->duty_cycle, car->wheels_ratio);
        set_direction(BACKWARD);
        vTaskDelay(pdMS_TO_TICKS(2000));
        // add_alarm_in_ms(2000, reverse_and_change, NULL, false);
        set_stop();
        change_state(car, IDLE);
        break;
    }
}

int64_t reverse_and_change(alarm_id_t id, void *user_data)
{
    printf("End of Obstacle State...\n");
    set_stop();
    change_state(global_car, IDLE);
    return 0;
}

void turn_with_interrupts(struct Car *car, enum DIRECTION direction, uint16_t no_of_interrupts)
{
    uint16_t starting_left_count = left_rising_edge_count;
    uint16_t starting_right_count = right_rising_edge_count;

    set_direction(direction);
    set_speed(car->duty_cycle, car->wheels_ratio);

    while (((left_rising_edge_count - starting_left_count) < no_of_interrupts) &&
           ((right_rising_edge_count - starting_right_count) < no_of_interrupts))
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    set_stop();
}

void state_exit(struct Car *car)
{
    switch (*(car->state))
    {
    case IDLE:
        break;
    case TRANSIT:
        set_stop();
        break;
    case ADJUST:
        break;
    case SCANNING:
        break;
    case OBSTACLE:
        set_stop();
        break;
    }
}

void change_state(struct Car *car, enum PID_STATE next_state)
{
    if (car->state != NULL)
    {
        state_exit(car);
    }
    else
    {
        car->state = (enum PID_STATE *)malloc(sizeof(enum PID_STATE));
        if (car->state == NULL)
        {
            printf("Malloc failed. Terminating program...\n");
            exit(1);
        }
    }

    *(car->state) = next_state;

    state_enter(car);
}

void car_init(struct Car *car)
{
    if (car == NULL)
    {
        printf("Malloc failed. Terminating program...\n");
        exit(1);
    }
    else
    {
        car->position = NULL;
        car->state = NULL;
        for (uint8_t i = 0; i < COMPONENTS_COUNT; i++)
        {
            car->components[i] = NULL;
        }
        car->wheels_ratio = 1.0;
        car->duty_cycle = 0.0;
    }
}

void components_init(struct Component *components[COMPONENTS_COUNT])
{
    for (uint8_t i = 0; i < COMPONENTS_COUNT; i++)
    {
        components[i] = (struct Component *)malloc(sizeof(struct Component));
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

        components[i]->task_handler = (TaskHandle_t *)malloc(sizeof(TaskHandle_t));
        if (components[i]->task_handler == NULL)
        {
            printf("Malloc failed. Terminating program...\n");
            exit(1);
        }
    }
}

void component_init(struct Component *component)
{
    if (component == NULL)
    {
        printf("Malloc failed. Terminating program...\n");
        exit(1);
    }
    else
    {
        component->buffer = NULL;
        component->task_handler = NULL;
        component->flags = NULL;
    }
}

void main_task(void *params)
{
    struct Car *car = (struct Car *)params;
    change_state(car, IDLE);
    printf("Message buffer address from main_task: %p\n", car->main_buffer);
    while (true)
    {
        state_execute(car);
    }
}

void vLaunch(struct Car *car)
{
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
    xTaskCreate(barcode_task, "barcode_task", configMINIMAL_STACK_SIZE, car, BARCODE_TASK_PRIORITY, car->components[BARCODE]->task_handler);
    xTaskCreate(ultrasonic_task, "ultrasonic_task", configMINIMAL_STACK_SIZE, car, ULTRASONIC_TASK_PRIORITY, car->components[ULTRASONIC]->task_handler);
    // xTaskCreate(infrared_task, "infrared_task", configMINIMAL_STACK_SIZE, car, INFRARED_TASK_PRIORITY, car->components[INFRARED]->task_handler);
    // xTaskCreate(magnetometer_task, "magnetometer_task", configMINIMAL_STACK_SIZE, car, MAGNETOMETER_TASK_PRIORITY, car->components[MAGNETOMETER]->task_handler);
    vTaskStartScheduler();
}

void init_wifi()
{
    cyw43_arch_init();
    cyw43_arch_enable_sta_mode();
    // Connect to the WiFI network - loop until connected
    while (cyw43_arch_wifi_connect_timeout_ms("Ryannnn", "asdfghjkl", CYW43_AUTH_WPA2_AES_PSK, 30000) != 0)
    {
        printf("Attempting to connect...\n");
    }
    // Print a success message once connected
    printf("Connected! \n");

    // Initialise web server
    httpd_init();
    printf("Http server initialised\n");

    // Configure SSI and CGI handler
    ssi_init();
    printf("SSI Handler initialised\n");
    cgi_init();
    printf("CGI Handler initialised\n");
}

int main()
{
    stdio_init_all();
    init_wifi();

    sleep_ms(5000);

    struct Car *car = (struct Car *)malloc(sizeof(struct Car));
    car_init(car);
    global_car = car;
    grid_init(car->grid);
    components_init(car->components);

    car->position = (struct Node **)malloc(sizeof(struct Node *));
    *(car->position) = car->grid[START_NODE_X][START_NODE_Y];

    vLaunch(car);
}