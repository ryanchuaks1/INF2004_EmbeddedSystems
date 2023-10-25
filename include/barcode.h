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
#define WALL_SENSOR_PIN 22
#define BARCODE_ADC_CHANNEL 0

#define DEBOUNCE_DELAY_MS 100


void barcode_init();
int get_ir_reading();
void check_if_wall();

enum BarType
{
    THICK_BLACK_BAR,
    THIN_BLACK_BAR,
    THICK_WHITE_BAR,
    THIN_WHITE_BAR
};

struct Flags
{
    bool start;
    bool end;
    bool black;
    bool white;
    bool thick;
    bool thin;
    int count;
};

static struct Flags barcodeFlags;
static enum BarType barType;
static uint64_t last_button_press_time = 0;

#endif // BARCODE_H
