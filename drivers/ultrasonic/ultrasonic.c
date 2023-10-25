#include <stdio.h>
#include <stdlib.h> // memory allocation
#include <time.h>	// time measurement

#include "pico/stdlib.h" // standard input/output operations
#include "hardware/timer.h"

#define BAUD_RATE 115200 // data transmission speed to 115200 bits per second

// Define the trigger and echo pins
#define TRIG_PIN 0 // GPIO 0
#define ECHO_PIN 1 // GPIO 1

// Speed of sound in air in centimeters/second
#define SPEED_OF_SOUND 34300
// Set to the actual system clock frequency
// Measure the distance in centimeters
float measure_distance()
{
	// Set the trigger pin high for 10 microseconds
	gpio_put(TRIG_PIN, 1);
	sleep_us(10); // Generate a precise delay of 10 microseconds
	gpio_put(TRIG_PIN, 0);
	// When trigger pin has bin triggered, the echo pin will be high and a pulse will be emmited

	// Wait for the echo pin to go high
	while (gpio_get(ECHO_PIN) == 0) tight_loop_contents();

	// Start the timer as the pusle is emmited
	uint32_t start_time = time_us_32();

	// Wait for the pulse to return
	while (gpio_get(ECHO_PIN) == 1);

	// Stop the timer when the pulse return
	uint32_t end_time = time_us_32();

	// Calculate the distance in centimeters
	// end_time is the time in seconds when the echo pin goes high
	// start_time is the time in seconds when the trigger pin goes high
	// speed_of_sound is the speed of sound in meters per second
	// 100 is the conversion factor from meters to centimeters

	float distance_cm = (end_time - start_time) * SPEED_OF_SOUND / (1000000 << 1);

	// Base on Research: https://uk.pi-supply.com/products/ultrasonic-distance-sensor-hc-sr04#:~:text=The%20HC%2DSR04%20sensor%20works,to%20the%20nearest%200.3cm.
	// The range of the HC-SR04 Sensor is between 2cm and 400cm
	// To prevent error, I will set the range to 4cm to 398cm so that I wouldn't measure the borders
	// return -1 on invalid range

	//printf("Distance: %.2fcm\n", distance_cm);

	return distance_cm > 398 || distance_cm < 4 ? -1 : distance_cm;
}

void ultrasonic_init()
{
	// Initialize trigger and echo pins
	gpio_init(TRIG_PIN);
	gpio_set_dir(TRIG_PIN, GPIO_OUT);
	gpio_init(ECHO_PIN);
	gpio_set_dir(ECHO_PIN, GPIO_IN);
}

void test_ultrasonic()
{
	float distance = measure_distance();
	printf("distance: %.2f\n", distance);
	if (distance < 0)
	{
		printf("Invalid Range\n");
	}
	else
	{
		printf("Distance: %.2f cm\n", distance);
	}
}

int main()
{
	// Initialize the Pico
	// stdio_init_all();
	stdio_usb_init();

	ultrasonic_init();	

	// Start a loop to measure the distance every second
	while (true)
	{
		test_ultrasonic();
		sleep_ms(1000);
	}

	return 0;
}
