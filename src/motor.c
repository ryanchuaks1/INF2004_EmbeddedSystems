/*
 * Author: Joshua Chua
 * Description: TODO: Add description
 */

#include "../include/motor.h"

/**
 * motor_task()
 * --------
 * Purpose: VTask for motor to be ran by freeRtos.
 *          Initialises the l298n
 *          Runs anything in the while loop
 * Arguments: params from vTask
 * Return: 1 as completed
 */
void motor_task(void* params)
{
    l298n_speed_pwm_setup(); // Initialise PWM for L298N

    while(true){
        //Nothing in the while loop, meant for future implementations
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

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
 * Purpose: Set the speed using the PWM for the car to move. 
 *          This function makes the car start
 * Arguments: duty_cycle of the PWM. Ratio = How much is one wheel faster than the other
 * Return: 1 as completed
 */
int set_speed(float duty_cycle, float ratio)
{
    // Divide the clock by 125Mhz / 200 = 625 000
    pwm_set_clkdiv(SPEED_SLICE_NUM, CLOCK_DIVIDER); 

    // Divide the Clock into 12500 Virtual Clock cycles
    // Frequency = 625 000 / 31250 = 20 Hz 
    pwm_set_wrap(SPEED_SLICE_NUM, WRAP_VALUE);

    // Set the Duty cycle of the PWN signal base on the ratio from Calibrations
    //left wheel = Channel A (ENA)
    //right wheel = Channel B (ENB)
    if(ratio > 1){

        //Increase the speed of the left wheel for straight line
        pwm_set_chan_level(SPEED_SLICE_NUM, PWM_CHAN_A, WRAP_VALUE * duty_cycle * (1 / ratio));
        pwm_set_chan_level(SPEED_SLICE_NUM, PWM_CHAN_B, WRAP_VALUE * duty_cycle);
    }
    else if (ratio < 1){
        //Lower the speed of the right wheel for straight line
        pwm_set_chan_level(SPEED_SLICE_NUM, PWM_CHAN_A, WRAP_VALUE * duty_cycle);
        pwm_set_chan_level(SPEED_SLICE_NUM, PWM_CHAN_B, WRAP_VALUE * duty_cycle * ratio);
    }
    else{
        //If Both the same, set to the same duty cycle each
        pwm_set_chan_level(SPEED_SLICE_NUM, PWM_CHAN_A, WRAP_VALUE * duty_cycle);
        pwm_set_chan_level(SPEED_SLICE_NUM, PWM_CHAN_B, WRAP_VALUE * duty_cycle);
    }

    pwm_set_enabled(0, true);

    return 1;
}

/**
 * set_direction()
 * --------
 * Purpose: Set the Direction of the car base on the DORECTION Enum. Useful for functions using the enum.
 *          Uses a Swtch case statement
 * Arguments: enum DIRECTION dir
 * Return: NULL
 */
void set_direction(enum DIRECTION dir){
    switch(dir){
        case FORWARD:
            printf("Forward...\n");
            set_forward();
            break;
        case BACKWARD:
            printf("Backward...\n");
            set_backward();
            gpio_put(L298N_INPUT_4, 1);
            break;
        case LEFT:
            printf("Left...\n");
            set_left();
            break;
        case RIGHT:
            printf("Right...\n");
            set_right();
            break;
    }
}

/**
 * set_forward()
 * --------
 * Purpose: Set the car to run forward by setting the GPIO pins on the controller
 * Arguments: NULL
 * Return: NULL
 */
void set_forward()
{
    gpio_put(L298N_INPUT_1, 1);
    gpio_put(L298N_INPUT_2, 0);
    gpio_put(L298N_INPUT_3, 1);
    gpio_put(L298N_INPUT_4, 0);
}

/**
 * set_backward()
 * --------
 * Purpose: Set the car to run backward by setting the GPIO pins on the controller
 * Arguments: NULL
 * Return: NULL
 */
void set_backward()
{
    gpio_put(L298N_INPUT_1, 0);
    gpio_put(L298N_INPUT_2, 1);
    gpio_put(L298N_INPUT_3, 0);
    gpio_put(L298N_INPUT_4, 1);
}

/**
 * set_left()
 * --------
 * Purpose: Set the car to turn left by setting the GPIO pins on the controller
 * Arguments: NULL
 * Return: NULL
 */
void set_left()
{
    gpio_put(L298N_INPUT_1, 0);
    gpio_put(L298N_INPUT_2, 1);
    gpio_put(L298N_INPUT_3, 1);
    gpio_put(L298N_INPUT_4, 0);
}

/**
 * set_right()
 * --------
 * Purpose: Set the car to turn right by setting the GPIO pins on the controller
 * Arguments: NULL
 * Return: NULL
 */
void set_right()
{
    gpio_put(L298N_INPUT_1, 1);
    gpio_put(L298N_INPUT_2, 0);
    gpio_put(L298N_INPUT_3, 0);
    gpio_put(L298N_INPUT_4, 1);
}


/**
 * set_stop()
 * --------
 * Purpose: Stops the car by setting the GPIO pins on the controller.
 * Arguments: NULL
 * Return: NULL
 */
void set_stop()
{
    gpio_put(L298N_INPUT_1, 0);
    gpio_put(L298N_INPUT_2, 0);
    gpio_put(L298N_INPUT_3, 0);
    gpio_put(L298N_INPUT_4, 0);
}