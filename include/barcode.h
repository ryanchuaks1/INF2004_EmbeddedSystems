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

// Define GPIO pin and ADC channel for the "front" IR sensor
#define BARCODE_SENSOR_PIN 26
#define BARCODE_ADC_CHANNEL 0
#define ADC_IRQ 0

#define THICK_WHITE 320
#define THIN_WHITE 1700

#define THICK_BLACK 3650
#define THIN_BLACK 1700

void barcode_init();
void check_front_IR_intensity();

enum BarTypeThreshold {
    THICK_BLACK_BAR,
    THIN_BLACK_BAR,
    THICK_WHITE_BAR,
    THIN_WHITE_BAR
};

static enum BarTypeThreshold barType = THICK_WHITE_BAR;
static volatile bool isBlackBar = false;

static int barResults[255];
static int resultCount = 0;
static int resetChecker = 0;

#endif // BARCODE_H
