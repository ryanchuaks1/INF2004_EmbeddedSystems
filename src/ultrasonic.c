#include "../include/ultrasonic.h"

// These variables are used to communicate between different parts of the code.
volatile bool measurement_completed = false;  // Flag indicating that a measurement has been completed.
volatile bool measure_distance_flag = false;   // Flag to trigger distance measurement.

// This function is called by a timer interrupt to mark the end of a measurement.
void timer_callback(struct repeating_timer *t)
{
    measurement_completed = true;
}

// This function handles GPIO interrupt events.
void gpio_callback(uint gpio, uint32_t events)
{
    // In this case, it's used to detect the rising and falling edges on the ECHO_PIN.
    // This is part of the ultrasonic distance measurement process.

    static uint32_t start_time = 0;  // Keeps track of the start time of a pulse.

    if (events & GPIO_IRQ_EDGE_RISE)
    {
        // When a rising edge is detected, it records the current time.
        start_time = time_us_32();
    }
    else if (events & GPIO_IRQ_EDGE_FALL)
    {
        // When a falling edge is detected, it calculates the pulse width, which is used to measure distance.
        uint32_t end_time = time_us_32();
        uint32_t pulse_width = end_time - start_time;
        float distance_cm = (float)pulse_width * SPEED_OF_SOUND / (2 * 1000000); // Calculate the distance in centimeters.

		// Base on Research: https://uk.pi-supply.com/products/ultrasonic-distance-sensor-hc-sr04#:~:text=The%20HC%2DSR04%20sensor%20works,to%20the%20nearest%200.3cm.
		// The range of the HC-SR04 Sensor is between 2cm and 400cm
		// To prevent error, I will set the range to 4cm to 398cm so that I wouldn't measure the borders
		// return -1 on invalid range
        if (distance_cm >= 4 && distance_cm <= 398)
        {
            printf("Distance: %.2f cm\n", distance_cm);
        }
        else
        {
            printf("Invalid Range\n");
        }
        measurement_completed = true;  // Marks the measurement as completed.
    }
}

// Initialize the ultrasonic sensor and set up GPIO interrupts for measuring distance.
void ultrasonic_init()
{
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);

    // Configure interrupt for the echo pin to detect rising and falling edges.
    gpio_set_irq_enabled_with_callback(ECHO_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
}

// This function is called by a timer interrupt to set the "measure_distance_flag."
void custom_timer_callback()
{
    measure_distance_flag = true;
}

// This is a wrapper function for the custom timer callback.
bool custom_timer_callback_wrapper(struct repeating_timer *rt)
{
    custom_timer_callback();
    return true;
}

// Wait for the measurement to complete using a custom delay.
void custom_delay_us(uint32_t us)
{
    while (!measurement_completed)
    {
        tight_loop_contents(); // Continue processing until the measurement is complete.
    }
    busy_wait_us(us);  // Use busy waiting for the specified time.
    measurement_completed = false; // Reset the measurement flag.
}

// Perform the ultrasonic distance measurement.
void test_ultrasonic()
{
    // Generate the trigger pulse.
    gpio_put(TRIG_PIN, 1);
    custom_delay_us(10); // Custom delay of 10 microseconds.
    gpio_put(TRIG_PIN, 0);

    // Wait for the measurement to complete.
    while (!measure_distance_flag)
    {
        tight_loop_contents();
    }
    measure_distance_flag = false;  // Reset the measurement flag.
}

int main()
{
    stdio_init_all();
    ultrasonic_init();

    // Configure and start a timer interrupt to trigger the distance measurement at regular intervals.
    struct repeating_timer timer;
    add_repeating_timer_us(500000, custom_timer_callback_wrapper, NULL, &timer); // 0.5-second interval

    // Start a loop to measure the distance.
    while (true)
    {
        test_ultrasonic();
    }

    return 0;
}
