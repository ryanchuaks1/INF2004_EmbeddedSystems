/*
 * Author: Farah
 * Description: TODO: Add description
 */
#ifndef ULTRASONIC_H
#define ULTRASONIC_H

// struct Ultrasonic{
//     MessageBufferHandle_t* buffer;
//     TaskHandle_t* task_handler;
// };

#define BAUD_RATE 115200 // data transmission speed to 115200 bits per second
#define TRIG_PIN 14 // GPIO 0
#define ECHO_PIN 15 // GPIO 1
#define SPEED_OF_SOUND 34300  // Speed of sound in air in centimeters/second
#define TRIGGERED_DISTANCE 50 //Distance to trigger clost object.
#define POLLING_TIME_MS 1000

#include "common.h"

float measure_distance();
void ultrasonic_init();
//bool measure_ultrasonic(struct repeating_timer *t);
void ultrasonic_task(void* params);

// void ultrasonic_init(void);
// void custom_delay_us(uint32_t us);
// void test_ultrasonic(void);
// void ultrasonic_task(void* params);
// bool custom_timer_callback_wrapper(struct repeating_timer *rt);

#endif
