/*
 * Author: Joshua Chua
 * Description: TODO: Add description
 */

/**
 * l298n_speed_pwm_setup()
 * --------
 * Purpose: Set up the PWM pins and the GPIO pins to control the direction using the L298N
 * Arguments: None
 * Return: 1 as completed
 */

#include "movement.h"

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
 * Arguments: - duty_cycle_left: Percentage of the duty cycle for the left motor
 *            - duty_cycle_left: Percentage of the duty cycle for the right motor
 * Return: 1 as completed
 */
int set_speed(uint8_t duty_cycle_left, uint8_t duty_cycle_right)
{
    // // Divide the clock by 125Mhz / 200 = 625 000
    pwm_set_clkdiv(SPEED_SLICE_NUM, CLOCK_DIVIDER); 

    // // Divide the Clock into 12500 Virtual Clock cycles
    // // Frequency = 625 000 / 31250 = 20 Hz as stated in the specifications
    pwm_set_wrap(SPEED_SLICE_NUM, WRAP_VALUE);

    // // Set the Duty cycle of the PWN signal to be 50% by dividing by 2 on channel A (PWM GPIO 2 is on Channel A)
    pwm_set_chan_level(SPEED_SLICE_NUM, PWM_CHAN_A, WRAP_VALUE/2);
    pwm_set_chan_level(SPEED_SLICE_NUM, PWM_CHAN_B, WRAP_VALUE/2);

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

//Sameple Main to run the function (To be Deleted After Submission)
// int main() {
//     stdio_init_all();
    
//     l298n_speed_pwm_setup();

//     set_speed(0.5, 0.5);

//     while (1)
//     {
//         printf("Looping...");
//         set_forward();

//         if(gpio_get(L298N_INPUT_1))
//         {
//             printf("Forward");
//         }
//         sleep_ms(1000);
//         set_backward();
//         if(gpio_get(L298N_INPUT_2))
//         {
//             printf("backward");
//         }
//         sleep_ms(1000);
        
//     }
// }
