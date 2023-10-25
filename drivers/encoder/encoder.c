#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define WHEEL_CIRCUMFERENCE 21.0 //Centimeters
#define NO_OF_HOLES 20.0
#define DISTANCE_BETWEEN_EDGE (WHEEL_CIRCUMFERENCE / NO_OF_HOLES)
#define SENSOR_GPIO 1
#define MEASURE_TIME 1000000 // 1 second in microseconds

volatile uint8_t rising_edge_count = 0;

void measure_edges(uint gpio, uint32_t events) {
        rising_edge_count++;  
}

void ir_sensor_init()
{
    gpio_init(SENSOR_GPIO);
    gpio_set_dir(SENSOR_GPIO, GPIO_IN);
    gpio_set_irq_enabled_with_callback(SENSOR_GPIO, GPIO_IRQ_EDGE_RISE, true, &measure_edges);
}

float read_speed(float start_time, float end_time)
{
    //Calculate current time in seconds
    float time_seconds = (end_time - start_time) / 1000000; 

    float speed_cm_s = (rising_edge_count * DISTANCE_BETWEEN_EDGE) / time_seconds;

    printf("Time: %.2fs\n", time_seconds);
    printf("Distance: %.2fcm\n", (float)(DISTANCE_BETWEEN_EDGE * rising_edge_count));
    printf("Speed: %.2f cm/s\n\n", speed_cm_s);
    rising_edge_count = 0;

    return speed_cm_s;
}

int main()
{
    stdio_init_all();

    ir_sensor_init(); 

    uint32_t start_time = time_us_32();

    while (true)
    {
        if (time_us_32() - start_time >= MEASURE_TIME){
            // float time_seconds = (time_us_32() - start_time) / 1000000; //Calculate current time in seconds

            // float speed_cm_s = (rising_edge_count * DISTANCE_BETWEEN_EDGE) / time_seconds;

            // printf("Speed: %.2f cm/s\n", speed_cm_s);
            // start_time = time_us_32();
            // rising_edge_count = 0;

            float speed = read_speed(start_time, time_us_32());
            start_time = time_us_32();
        }
    }
    
    return 0;

}

