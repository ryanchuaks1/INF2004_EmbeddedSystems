/*
 * Author: Joshua Chua
 * Description: Main file for the project
 */
#include "../include/wheel_encoder.h"

//Global Variable: Interrupts variable to count the left and right wheel rising edges
uint16_t left_rising_edge_count;
uint16_t right_rising_edge_count;

/**
 * wheel_encoder_task()
 * --------
 * Purpose: VTask for wheel encoders to be ran by freeRtos.
 *          Initialises the encoders
 * Arguments: params from vTask
 * Return: 1 as completed
 */
void wheel_encoder_task(void* params){

    wheel_encoder_init();
    printf("Wheel encoder initialized\n");

    while(true){
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/**
 * wheel_encoder_init()
 * --------
 * Purpose: Initializes the Wheel Encoder Pins
 *          Sets the number of rising edges for each encoder to 0
 * Arguments: None
 * Return: None
 */
void wheel_encoder_init()
{
    gpio_init(LEFT_ENCODER_GPIO);
    gpio_set_dir(LEFT_ENCODER_GPIO, GPIO_IN);
    gpio_set_irq_enabled_with_callback(LEFT_ENCODER_GPIO, GPIO_IRQ_EDGE_RISE, true, &interrupt_callback);
    left_rising_edge_count = 0;
    
    gpio_init(RIGHT_ENCODER_GPIO);
    gpio_set_dir(RIGHT_ENCODER_GPIO, GPIO_IN);
    gpio_set_irq_enabled_with_callback(RIGHT_ENCODER_GPIO, GPIO_IRQ_EDGE_RISE, true, &interrupt_callback);
    right_rising_edge_count = 0;
}

/**
 * read_speed()
 * --------
 * Purpose: Reads the speed between the start and the end time
 *          Distance between the time intervals are calculated and added to total distances
 * Arguments: float start_time, end_time and the variable to store the total distance
 * Return: The current speed for the time intervals
 */
float read_speed(float start_time, float end_time, float* total_distance)
{
    //Calculate current time in seconds
    float time_seconds = (end_time - start_time) / 1000000;

    //Calculate the average number of rising edges base on the bot right and left encoders
    float no_rising_edges = ((left_rising_edge_count + right_rising_edge_count) / 2 );

    //Calculate the Speed
    float speed_cm_s = ( no_rising_edges * DISTANCE_BETWEEN_EDGE) / time_seconds;

    //Calculate the current distances
    *total_distance = (float)(DISTANCE_BETWEEN_EDGE * no_rising_edges);

    //Print the Statistics
    printf("Time: %.2fs\n", time_seconds);
    printf("Distance: %.2fcm\n", *total_distance);
    printf("Total Distance: %.2fcm\n", *total_distance);
    printf("Speed: %.2f cm/s\n\n", speed_cm_s);
    return speed_cm_s;
}
