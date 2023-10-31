/*
 * Author: Farah
 * Description: TODO: Add description
 */
#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "common.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

#define BAUD_RATE 115200
#define TRIG_PIN 0
#define ECHO_PIN 1
#define SPEED_OF_SOUND 34300

void ultrasonic_init();
void test_ultrasonic();
#endif
