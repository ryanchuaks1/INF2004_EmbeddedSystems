/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Output PWM signals on pins 0 and 1

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

/**
 * l298n_speed_pwm_setup()
 * --------
 * Purpose: Set up the PWM pins and the GPIO pins to control the direction using the L298N
 * Arguments: None
 * Return: 1 as completed
 */
int l298n_speed_pwm_setup()
{
    // Tell GPIO 0 and 1 they are allocated to the PWM
    gpio_set_function(L298N_PIN_ENA, GPIO_FUNC_PWM);
    gpio_set_function(L298N_PIN_ENB, GPIO_FUNC_PWM);

    // Serial Input Output Configuration
    gpio_set_function(L298N_INPUT_1, GPIO_FUNC_SIO);
    gpio_set_function(L298N_INPUT_2, GPIO_FUNC_SIO);
    gpio_set_function(L298N_INPUT_3, GPIO_FUNC_SIO);
    gpio_set_function(L298N_INPUT_4, GPIO_FUNC_SIO);

    //Set all the relevant GPIO pins to output for the L298N
    gpio_set_dir(L298N_INPUT_1, GPIO_OUT);
    gpio_set_dir(L298N_INPUT_2, GPIO_OUT);
    gpio_set_dir(L298N_INPUT_3, GPIO_OUT);
    gpio_set_dir(L298N_INPUT_4, GPIO_OUT);

    return 1;
}

/**
 * set_speed()
 * --------
 * Purpose: Set the speed using the PWM for the car to move
 * Arguments: - duty_cycle
 * Return: 1 as completed
 */
int set_speed(float duty_cycle)
{
    // Divide the clock by 125Mhz / 200 = 625 000
    pwm_set_clkdiv(SPEED_SLICE_NUM, CLOCK_DIVIDER); 

    // Divide the Clock into 12500 Virtual Clock cycles
    // Frequency = 625 000 / 31250 = 20 Hz as stated in the specifications
    pwm_set_wrap(SPEED_SLICE_NUM, WRAP_VALUE);

    // et the Duty cycle of the PWN signal * duty cycle on channel A (PWM GPIO 2 is on Channel A)
    pwm_set_chan_level(SPEED_SLICE_NUM, PWM_CHAN_A, WRAP_VALUE * duty_cycle);
    pwm_set_chan_level(SPEED_SLICE_NUM, PWM_CHAN_B, WRAP_VALUE * duty_cycle);

    pwm_set_enabled(0, true);

    return 1;
}

// Set the car to run forward
void set_forward()
{
    gpio_put(L298N_INPUT_1, 1);
    gpio_put(L298N_INPUT_2, 0);
    gpio_put(L298N_INPUT_3, 1);
    gpio_put(L298N_INPUT_4, 0);
}

// Set the car to run backward
void set_backward()
{
    gpio_put(L298N_INPUT_1, 0);
    gpio_put(L298N_INPUT_2, 1);
    gpio_put(L298N_INPUT_3, 0);
    gpio_put(L298N_INPUT_4, 1);
}

// Set the car to turn left
void set_left()
{
    gpio_put(L298N_INPUT_1, 1);
    gpio_put(L298N_INPUT_2, 0);
    gpio_put(L298N_INPUT_3, 1);
    gpio_put(L298N_INPUT_4, 0);
}

// Set the car to turn right
void set_right()
{
    gpio_put(L298N_INPUT_1, 0);
    gpio_put(L298N_INPUT_2, 1);
    gpio_put(L298N_INPUT_3, 0);
    gpio_put(L298N_INPUT_4, 1);
}

// Stop the car
void set_stop()
{
    gpio_put(L298N_INPUT_1, 0);
    gpio_put(L298N_INPUT_2, 0);
    gpio_put(L298N_INPUT_3, 0);
    gpio_put(L298N_INPUT_4, 0);
}

void test_movement()
{
    
    set_forward();
    printf("Forward");
    sleep_ms(1000);

    set_right();
    printf("Right");
    sleep_ms(1000);

    set_forward();
    printf("Forward");
    sleep_ms(1000);

    set_backward();
    printf("Backward");
    sleep_ms(1000);

    set_left();
    printf("Left");
    sleep_ms(1000);

    set_backward();
    printf("Backward");
    sleep_ms(1000);

}


int main() {
    stdio_init_all();
    
    l298n_speed_pwm_setup();

    set_speed(0.5);

    while (1)
    {   
        printf("Looping...");
        test_movement();
    }
}


