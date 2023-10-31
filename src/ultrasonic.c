#include "ultrasonic.h"
#include <stdio.h>
#include <stdlib.h>
#include "hardware/gpio.h"

volatile bool measurement_completed = false;
volatile bool measure_distance_flag = false;

void timer_callback(struct repeating_timer *t)
{
    measurement_completed = true;
}

void gpio_callback(uint gpio, uint32_t events)
{
    static uint32_t start_time = 0;

    if (events & GPIO_IRQ_EDGE_RISE)
    {
        start_time = time_us_32();
    }
    else if (events & GPIO_IRQ_EDGE_FALL)
    {
        uint32_t end_time = time_us_32();
        uint32_t pulse_width = end_time - start_time;
        float distance_cm = pulse_width * SPEED_OF_SOUND / (2 * 1000000);
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
    gpio_set_irq_enabled_with_callback(ECHO_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
}

void custom_timer_callback()
{
    measure_distance_flag = true;
}

bool custom_timer_callback_wrapper(struct repeating_timer *rt)
{
    custom_timer_callback();
    return true;
}

void custom_delay_us(uint32_t us)
{
    while (!measurement_completed)
    {
        tight_loop_contents();
    }
    busy_wait_us(us);
    measurement_completed = false;
}

void test_ultrasonic()
{
    gpio_put(TRIG_PIN, 1);
    custom_delay_us(10);
    gpio_put(TRIG_PIN, 0);

    while (!measure_distance_flag)
    {
        tight_loop_contents();
    }
    measure_distance_flag = false;
}


int main()
{
    stdio_init_all();
    ultrasonic_init();

    struct repeating_timer timer;
    add_repeating_timer_us(500000, custom_timer_callback_wrapper, NULL, &timer);

    while (true)
    {
        test_ultrasonic();
    }

    return 0;
}
