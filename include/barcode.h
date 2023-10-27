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

#define BARCODE_THRESHOLD 1000

void barcode_init();
int get_ir_reading();
void check_if_wall();
void init_read_barcode();
void read_barcode();

enum BarType
{
    BLACK_BAR,
    WHITE_BAR
};

struct Flags
{
    bool isPrevBlackBar;
    bool isBarcode;
    int count;
    int limitter;
};

static struct Flags barcodeFlags;
static enum BarType barType;
static uint64_t last_button_press_time = 0;
static uint16_t coded_barcode = 0;
static uint64_t decoded_barcode = 0;

static int black_bar_times[] = {0, 0, 0, 0, 0}; // Array for black bar times
static int white_bar_times[] = {0, 0, 0, 0, 0};    // Array for white bar times
static int bar_index = 0;                          // Index for the current bar

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)       \
    ((byte)&0x80 ? '1' : '0'),     \
        ((byte)&0x40 ? '1' : '0'), \
        ((byte)&0x20 ? '1' : '0'), \
        ((byte)&0x10 ? '1' : '0'), \
        ((byte)&0x08 ? '1' : '0'), \
        ((byte)&0x04 ? '1' : '0'), \
        ((byte)&0x02 ? '1' : '0'), \
        ((byte)&0x01 ? '1' : '0')

#endif // BARCODE_H
