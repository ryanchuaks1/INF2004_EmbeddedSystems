/*
 * Author: Joshua Chua
 * Description: Main file for the project
 */
#include "../include/wheel_encoder.h"

void wheel_encoder_task(void* params){
    wheel_encoder_init();

    struct Car* car = (struct Car*)params;

    size_t xBytesReceived;
    size_t xBytesSent;

    while(true){

    }

}

void measure_edges(uint gpio, uint32_t events) {
    if(gpio == LEFT_ENCODER_GPIO){
        left_rising_edge_count++;
    }
    else if (gpio == RIGHT_ENCODER_GPIO){
        right_rising_edge_count++;
    }
    encoder_interrupt_count++;
}

void wheel_encoder_init()
{
    gpio_init(SENSOR_GPIO);
    gpio_set_dir(SENSOR_GPIO, GPIO_IN);
    gpio_set_irq_enabled_with_callback(SENSOR_GPIO, GPIO_IRQ_EDGE_RISE, true, &measure_edges);
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

