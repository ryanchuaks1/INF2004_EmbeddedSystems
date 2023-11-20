#include "../include/ultrasonic.h"

// Set to the actual system clock frequency
// Measure the distance in centimeters
struct repeating_timer ultrasonic_timer;

float measure_distance()
{
	// Set the trigger pin high for 10 microseconds
	gpio_put(TRIG_PIN, 1);
	busy_wait_us(10); // Sleep for 10 milliseconds, Generate a precise delay of 10 microseconds (Use Busy Wait as we dont want any process using the 10ms or will cuase in accuracy)
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

bool measure_ultrasonic(struct repeating_timer *t) 
{
    float distance = measure_distance();
    printf("Distance measured: %.2f\n", distance);
    if (distance > 0 && distance <= TRIGGERED_DISTANCE)
    {
        uint8_t message = ULTRASONIC;
        xMessageBufferSend(
            *(global_car->main_buffer),
            (void *)&message,
            sizeof(message),
            portMAX_DELAY
        );

        printf("Obstacle detected, changing state\n");

        bool cancelled = cancel_repeating_timer(&ultrasonic_timer);
    
    }
    return true;
}

void ultrasonic_task(void* params)
{
    printf("Start Ultrasonic\n");
    ultrasonic_init();
    float distance = -1;
    
    while (1)
    {
        printf("Run Functions\n");
        struct Car* car = (struct Car*)params;
        uint16_t duration_ms = 10000;
        xMessageBufferReceive(
            *(car->components[ULTRASONIC]->buffer), 
            &duration_ms, 
            sizeof(duration_ms), 
            portMAX_DELAY
            );

        printf("Duration: %d\n", duration_ms);

        //After recieving the duratuon, we know to start the function
        vTaskDelay(pdMS_TO_TICKS(POLLING_TIME_MS));

        add_repeating_timer_ms(-(POLLING_TIME_MS), measure_ultrasonic, NULL, &ultrasonic_timer);
        
        // //After recieving the duratuon, we know to start the function
        // uint32_t end_time = time_us_32() + duration_ms;

        // while (time_us_32() < end_time)
        // {
        //     distance = measure_distance();
        //     printf("Distance measured: %.2f\n", distance);
        //     if (distance > 0 && distance >= TRIGGERED_DISTANCE)
        //     {
        //         uint8_t message = ULTRASONIC;
        //         xMessageBufferSend(
        //             *(global_car->main_buffer),
        //             (void *)&message,
        //             sizeof(message),
        //             portMAX_DELAY
        //         );

        //         printf("Obstacle detected, changing state\n");
        //     }

        //     vTaskDelay(pdMS_TO_TICKS(POLLING_TIME_MS));
        // } 

        // printf("Finish Ultrasonic...\n");
    }
    
}