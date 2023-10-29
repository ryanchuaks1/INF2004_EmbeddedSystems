#include "../include/ultrasonic.h"

volatile bool measurement_completed = false;

void timer_callback(struct repeating_timer *t)
{
    measurement_completed = true;
}

void gpio_callback(uint gpio, uint32_t events)
{
    // Handle GPIO interrupt events
    // In this case, it can be used to detect the rising and falling edges on the ECHO_PIN
    // Start and stop a timer to measure the pulse width for distance calculation
    static uint32_t start_time = 0;
    if (events & GPIO_IRQ_EDGE_RISE)
    {
        start_time = time_us_32();
    }
    else if (events & GPIO_IRQ_EDGE_FALL)
    {
        uint32_t end_time = time_us_32();
        uint32_t pulse_width = end_time - start_time;
        // Calculate the distance based on the pulse width
        float distance_cm = pulse_width * SPEED_OF_SOUND / (2 * 1000000); // in centimeters
        if (distance_cm >= 4 && distance_cm <= 398)
        {
            printf("Distance: %.2f cm\n", distance_cm);
        }
        else
        {
            printf("Invalid Range\n");
        }
        measurement_completed = true;
    }
}

void ultrasonic_init()
{
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);

    // Configure interrupt for the echo pin
    gpio_set_irq_enabled_with_callback(ECHO_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
}

void test_ultrasonic()
{
    // Generate the trigger pulse
    gpio_put(TRIG_PIN, 1);
    sleep_us(10); // Generate a precise delay of 10 microseconds
    gpio_put(TRIG_PIN, 0);

    // Wait for the measurement to complete using the custom interrupt
    measurement_completed = false;
    while (!measurement_completed)
    {
        tight_loop_contents();
    }
}

int main()
{
    stdio_init_all();
    ultrasonic_init();

    // Configure and start the timer interrupt
    struct repeating_timer timer;
    add_repeating_timer_us(1000000, timer_callback, NULL, &timer); // 1-second interval

    // Start a loop to measure the distance
    while (true)
    {
        test_ultrasonic();
    }

    return 0;
}