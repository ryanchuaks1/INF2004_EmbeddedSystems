/*
 * Author: Joshua Chua
 * Description: TODO: Add description
 */
#ifndef MOVEMENT_H
#define MOVEMENT_H

#endif // MOVEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

// Direction Pins
#define L298N_INPUT_1 2
#define L298N_INPUT_2 3
#define L298N_INPUT_3 4
#define L298N_INPUT_4 5

// Speed GPIO Pins
#define L298N_PIN_ENA 0 //Channel: 0A
#define L298N_PIN_ENB 1 //Channel: 0B
#define SPEED_SLICE_NUM 0

//Frequency & Speed Variables
// 125Mhz/ CLOCK_DIVIDER * WRAP_VALUE = Frequency wanted, currently, 20Hz is enough.
// For Documentation: Lower the frequency, the lesser power, the lesser the reactivity.
#define CLOCK_DIVIDER 200
#define WRAP_VALUE 31250

int l298n_speed_pwm_setup();
int set_speed(float duty_cycle);
void set_forward();
void set_backward();
void set_left();
void set_right();
void set_stop();
