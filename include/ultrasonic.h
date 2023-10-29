/*
 * Author: Farah
 * Description: TODO: Add description
 */
#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "common.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"

#define BAUD_RATE 115200
#define TRIG_PIN 0
#define ECHO_PIN 1
#define SPEED_OF_SOUND 34300

#define TRIG_PIN 0
#define ECHO_PIN 1
#define SPEED_OF_SOUND 34300

void ultrasonic_init();
void test_ultrasonic();
void timer_callback(struct repeating_timer *t);
void gpio_callback(uint gpio, uint32_t events);

#endif
