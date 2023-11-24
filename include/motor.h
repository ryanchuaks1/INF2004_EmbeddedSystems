/*
 * Author: Joshua Chua
 * Description: TODO: Add description
 */
#ifndef MOTOR_H
#define MOTOR_H

#include "hardware/pwm.h"
#include "common.h"

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

// struct Motor{
//     MessageBufferHandle_t* buffer;
//     TaskHandle_t* task_handler;
// };

int l298n_speed_pwm_setup();
void set_speed(float left_wheel_duty_cycle, float right_wheel_duty_cycle);
void set_forward();
void set_backward();
void set_left();
void set_right();
void set_stop();
void set_hard_left_back();
void set_hard_right_back();
void set_hard_left_forward();
void set_hard_right_forward();

static bool motor_finish = false;


#endif