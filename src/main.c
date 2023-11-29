/*
 * Author: Ryan Chua
 * Description: Main file for the project
 */

#include "../include/main.h"

struct Car *global_car;

/*
 * state_enter()
 * ---------------------
 * Purpose: Runs when transitioning into a new state. What it runs is dependent on the current state
 * Arguments: car struct
 * Returns: void
 */
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

/*
 * state_execute()
 * ---------------------
 * Purpose: Runs continuously inside the main_task. What it runs is dependent on the current state
 * Arguments: car struct
 * Returns: void
 */
void state_execute(struct Car *car)
{
    size_t xBytesSent;
    size_t xBytesReceived;

    // for identifying the component which sends the message into the buffer
    uint8_t opcode = 10;

    uint16_t starting_left_count;
    uint16_t starting_right_count;

    static uint16_t duration_ms = 10000;
    static enum DIRECTION direction = FORWARD;

    // number of interrupts on the wheel encoder to complete a turn
    uint8_t turn_interrupt_count = 10;

    switch (*(car->state))
    {
    /*
    * IDLE state
    * ---------------------
    * Purpose: For setting up/configuring/initializing the car. Set the duty cycle and calibrate the car before it starts moving
    */
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
    /*
    * TRANSIT state
    * ---------------------
    * Purpose: This state is only used when the car is in transit (moving), it will send messages to the different components
    * to start taking measurements, and listen for messages while the car is in transit (so that the TRANSIT state can be interrupted)
    * 1. If we receive a message from INFRARED, it means that we have detected a black line, so we reverse our car and turn
    * 2. If we receive a message from ULTRASONIC, it means that we have detected an object, hence the car needs to reverse
    * 3. If we receive a message from BARCODE, it means that we have detected a barcode, hence we reverse the car, ask the barcode to start reading
    * and move the car at a slow but constant speed.
    */
    case TRANSIT:

        //Send message to the components that they can start sensing, we send the duration planned for the car in transit state for the components if they need it
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

        //Set the car to move at the direction specified
        uint32_t start_time_us = time_us_32();
        set_direction(direction);
        set_speed(car->duty_cycle, car->wheels_ratio);

        //Recieve any opcodes from the components
        xMessageBufferReceive(
            *(car->main_buffer),
            (void *)&opcode,
            sizeof(opcode),
            pdMS_TO_TICKS(duration_ms));
        printf("Received: %f\n", (float)(time_us_32() - start_time_us) / 1000);

        //Process the opcodes and switch states if needed
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
        }
        else if (opcode == ULTRASONIC)
        {
            printf("Activate Ultraconic State....");
            change_state(car, OBSTACLE);
        }
        else if ((float)(time_us_32() - start_time_us) / 1000 >= (float)duration_ms)
        {
            printf("Finished...\n");
            change_state(car, IDLE);
        }

        break;
    /*
    * ADJUST state
    * ---------------------
    * Purpose: State for adjusting the car to the direction specified
    */
    case ADJUST:

        turn_with_interrupts(car, direction, turn_interrupt_count);    
        change_state(car, IDLE);

        break;
    /*
    * SCANNING state
    * ---------------------
    * Purpose: Car will reverse and task the barcode_task to start reading for inputs, then we move the car at a constant speed to read
    * the barcode.
    */
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
    /*
    * OBSTACLE state
    * ---------------------
    * Purpose: Reverses the car upon the detection of obstacles.
    */
    case OBSTACLE:
        printf("Inside Obstacle state\n");

        //Reverse the Car for 2 seconds upon obstacle detection
        set_speed(car->duty_cycle, car->wheels_ratio);
        set_direction(BACKWARD);
        vTaskDelay(pdMS_TO_TICKS(2000));
        set_stop();

        change_state(car, IDLE);
        break;
    }
}

/*
 * turn_with_interrupts()
 * ---------------------
 * Purpose: Turns the Car to Left or Right in 90 degrees
 * Arguments: car struct, the direction and the number of interrupts to stop turning
 * Returns: None
 */
void turn_with_interrupts(struct Car *car, enum DIRECTION direction, uint16_t no_of_interrupts)
{
    uint16_t starting_left_count = left_rising_edge_count;
    uint16_t starting_right_count = right_rising_edge_count;

    set_direction(direction);
    set_speed(car->duty_cycle, car->wheels_ratio);

    //We use the Interrupt count to turn the wheels
    //From testing, around 10 interrupts are needed for our to turn 90 degrees
    while (((left_rising_edge_count - starting_left_count) < no_of_interrupts) &&
           ((right_rising_edge_count - starting_right_count) < no_of_interrupts))
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    set_stop();
}

/*
 * state_exit()
 * ---------------------
 * Purpose: Runs the exit function for the current state when transitioning into a new state.
 * Arguments: car struct
 * Returns: void
 */
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

/*
 * change_state()
 * ---------------------
 * Purpose: Called when the car needs to change from 1 state or the other.
 * 1. Runs the exit function for the current state
 * 2. Set the current state to be the next state (execute() will be run by the main_task automatically)
 * 3. Runs the entry function for the next state
 * Arguments: car struct, and the enum for the next state
 * Returns: void
 */
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

/*
 * car_init()
 * ---------------------
 * Purpose: Checks if the car struct exist and sets the default values
 * Arguments: car struct
 * Returns: void
 */
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

/*
 * components_init()
 * ---------------------
 * Purpose: create component struct and Task Handler for each components
 * Arguments: List of Components
 * Returns: void
 */
void components_init(struct Component *components[COMPONENTS_COUNT])
{
    for (uint8_t i = 0; i < COMPONENTS_COUNT; i++)
    {
        components[i] = (struct Component *)malloc(sizeof(struct Component));
        component_init(components[i]);

        components[i]->task_handler = (TaskHandle_t *)malloc(sizeof(TaskHandle_t));
        if (components[i]->task_handler == NULL)
        {
            printf("Malloc failed. Terminating program...\n");
            exit(1);
        }
    }
}

/*
 * component_init()
 * ---------------------
 * Purpose: For the component struct, as its variables to the default values
 * Arguments: Component Struct
 * Returns: void
 */
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

/*
 * main_task()
 * ---------------------
 * Purpose: The main task for the car
 * 1. Set car state to IDLE
 * 2. Always runs the execute() function
 * Arguments: car struct
 * Returns: void
 */
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
    MessageBufferHandle_t pathfinding_buffer = xMessageBufferCreate(mbaTASK_MESSAGE_BUFFER_SIZE);

    car->components[BARCODE]->buffer = &barcode_buffer;
    car->components[INFRARED]->buffer = &infrared_buffer;
    car->components[MAGNETOMETER]->buffer = &magnetometer_buffer;
    car->components[MAPPING]->buffer = &mapping_buffer;
    car->components[MOTOR]->buffer = &motor_buffer;
    car->components[ULTRASONIC]->buffer = &ultrasonic_buffer;
    car->components[WHEEL_ENCODER]->buffer = &wheel_encoder_buffer;
    car->components[PATHFINDING]->buffer = &pathfinding_buffer;
    car->main_buffer = &main_buffer;

    //xTaskCreate(main_task, "main_task", configMINIMAL_STACK_SIZE, (void *)car, MAIN_TASK_PRIORITY, NULL);
    xTaskCreate(mapping_task, "mapping_task", configMINIMAL_STACK_SIZE, car, MAPPING_TASK_PRIORITY, car->components[MAPPING]->task_handler);
    xTaskCreate(pathfinding_task, "pathfinding_task", configMINIMAL_STACK_SIZE, car, PATHFINDING_TASK_PRIORITY, car->components[PATHFINDING]->task_handler);
    //xTaskCreate(motor_task, "motor_task", configMINIMAL_STACK_SIZE, (void *)car, MOTOR_TASK_PRIORITY, car->components[MOTOR]->task_handler);
    //xTaskCreate(wheel_encoder_task, "wheel_encoder_task", configMINIMAL_STACK_SIZE, (void *)car, WHEEL_ENCODER_TASK_PRIORITY, car->components[WHEEL_ENCODER]->task_handler);
    //xTaskCreate(barcode_task, "barcode_task", configMINIMAL_STACK_SIZE, car, BARCODE_TASK_PRIORITY, car->components[BARCODE]->task_handler);
    //xTaskCreate(ultrasonic_task, "ultrasonic_task", configMINIMAL_STACK_SIZE, car, ULTRASONIC_TASK_PRIORITY, car->components[ULTRASONIC]->task_handler);
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
    //init_wifi();

    sleep_ms(5000);

    struct Car *car = (struct Car *)malloc(sizeof(struct Car));
    car_init(car);
    global_car = car;
    grid_init(car->grid, false);
    grid_init(car->explored_grid, true);
    components_init(car->components);

    car->position = (struct Node **)malloc(sizeof(struct Node *));
    *(car->position) = car->grid[START_NODE_X][START_NODE_Y];

    add_wall(car->grid, car->grid[0][0], EAST);
    add_wall(car->grid, car->grid[3][0], EAST);
    add_wall(car->grid, car->grid[2][1], NORTH | EAST | SOUTH);
    add_wall(car->grid, car->grid[3][1], WEST);
    add_wall(car->grid, car->grid[0][2], SOUTH);
    add_wall(car->grid, car->grid[1][2], EAST);
    add_wall(car->grid, car->grid[2][2], EAST);
    add_wall(car->grid, car->grid[3][2], EAST);
    add_wall(car->grid, car->grid[1][3], EAST);
    add_wall(car->grid, car->grid[2][3], EAST);
    add_wall(car->grid, car->grid[3][3], EAST);
    add_wall(car->grid, car->grid[1][4], NORTH | SOUTH);
    add_wall(car->grid, car->grid[2][5], SOUTH);

    vLaunch(car);
}