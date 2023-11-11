#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define WHEEL_CIRCUMFERENCE 21.0 //Centimeters
#define NO_OF_HOLES 20.0
#define DISTANCE_BETWEEN_EDGE (WHEEL_CIRCUMFERENCE / NO_OF_HOLES)
#define SENSOR_GPIO 1
//#define MEASURE_TIME 1000000 // 1 second in microseconds

volatile uint8_t rising_edge_count = 0;