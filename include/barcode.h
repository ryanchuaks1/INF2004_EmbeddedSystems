/*
 * Author: Ryan Chua
 * Description: Define pins to be used, threshold for barcode sensor,
 * and functions to read barcode
 */

#ifndef BARCODE_H
#define BARCODE_H

#include "common.h"

// every barcode char is 9 bars
// start w black end with black
// 5 black bar (+0-9)
// 4 white bar (+0,+10,+20,+30)
// narrow = 0
// thick = 1

#define BARCODE_SENSOR_PIN 26
#define BARCODE_ADC_CHANNEL 0
#define BARCODE_SENSOR_THRESHOLD 100

void barcode_init();
char read_barcode();
int read_intensity();

#endif // BARCODE_H