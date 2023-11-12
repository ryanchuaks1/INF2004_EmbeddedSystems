#ifndef WHEEL_ENCODER_H
#define WHEEL_ENCODER_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "common.h"

#define WHEEL_CIRCUMFERENCE 21.0 //Centimeters
#define NO_OF_HOLES 20.0
#define DISTANCE_BETWEEN_EDGE (WHEEL_CIRCUMFERENCE / NO_OF_HOLES)
#define SENSOR_GPIO 1
//#define MEASURE_TIME 1000000 // 1 second in microseconds

// Use this instead of global variables, then store the address of this into the Component's (void* flags) variable
struct Wheel_Encoder_Flags{
    volatile uint8_t rising_edge_count;
};

volatile static uint8_t encoder_interrupt_count = 0;

void wheel_encoder_task(void* params);
void measure_edges(uint gpio, uint32_t events);
void wheel_encoder_init();
float read_speed(float start_time, float end_time, float* total_distance);

#endif