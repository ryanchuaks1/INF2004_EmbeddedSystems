/*
 * Author: Joshua Chua
 * Description: Main file for the project
 */
#include "../include/wheel_encoder.h"

uint16_t left_rising_edge_count;
uint16_t right_rising_edge_count;

void wheel_encoder_task(void* params){
    wheel_encoder_init();

    struct Car* car = (struct Car*)params;

    size_t xBytesReceived;
    size_t xBytesSent;

    uint8_t turn_interrupt_count = 0;

    printf("Wheel encoder initialized\n");
    while(true){
        // xBytesReceived = xMessageBufferReceive(
        //     *(car->main_buffer),
        //     (void*)&turn_interrupt_count,
        //     sizeof(turn_interrupt_count),
        //     portMAX_DELAY
        // );

        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}

// void measure_edges(uint gpio, uint32_t events) {
//     if(gpio == LEFT_ENCODER_GPIO){
//         left_rising_edge_count++;
//     }
//     else if (gpio == RIGHT_ENCODER_GPIO){
//         right_rising_edge_count++;
//     }
//     encoder_interrupt_count++;
// }

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

float read_speed(float start_time, float end_time, float* total_distance)
{
    //Calculate current time in seconds
    float time_seconds = (end_time - start_time) / 1000000;

    float speed_cm_s = (encoder_interrupt_count * DISTANCE_BETWEEN_EDGE) / time_seconds;

    float current_distance = (float)(DISTANCE_BETWEEN_EDGE * encoder_interrupt_count);

    *total_distance += current_distance;

    printf("Time: %.2fs\n", time_seconds);
    printf("Distance: %.2fcm\n", current_distance);
    printf("Total Distance: %.2fcm\n", *total_distance);
    printf("Speed: %.2f cm/s\n\n", speed_cm_s);
    encoder_interrupt_count = 0;
    return speed_cm_s;
}

// int main()
// {
//     stdio_init_all();

//     ir_sensor_init(); 

//     uint32_t start_time = time_us_32();
//     float total_distance = 0;

//     while (true)
//     {
//         if (time_us_32() - start_time >= MEASURE_TIME){
//             // float time_seconds = (time_us_32() - start_time) / 1000000; //Calculate current time in seconds

//             // float speed_cm_s = (rising_edge_count * DISTANCE_BETWEEN_EDGE) / time_seconds;

//             // printf("Speed: %.2f cm/s\n", speed_cm_s);
//             // start_time = time_us_32();
//             // rising_edge_count = 0;

//             float speed = read_speed(start_time, time_us_32(), &total_distance);
//             start_time = time_us_32();
//         }
//     }
    
//     return 0;

// }

