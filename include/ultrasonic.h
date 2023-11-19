/*
 * Author: Farah
 * Description: TODO: Add description
 */
#ifndef ULTRASONIC_H
#define ULTRASONIC_H

// struct Ultrasonic{
//     MessageBufferHandle_t* buffer;
//     TaskHandle_t* task_handler;
// };

#define BAUD_RATE 115200 // data transmission speed to 115200 bits per second
#define TRIG_PIN 0 // GPIO 0
#define ECHO_PIN 1 // GPIO 1
#define SPEED_OF_SOUND 34300  // Speed of sound in air in centimeters/second

void ultrasonic_init(void);
void custom_delay_us(uint32_t us);
void test_ultrasonic(void);
void ultrasonic_task(void* params);

#endif
