#include <stdio.h>


#include "pico/stdlib.h"
#include "pico/stdio_usb.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"

#include "pico/binary_info.h"
#include "macropad_usb.h"


extern const uint PIN_MUX_ENABLE;
extern const uint PIN_MUX_A;
extern const uint PIN_MUX_B;
extern const uint PIN_MUX_C;

extern const uint PIN_KD0;
extern const uint PIN_KD1;
extern const uint PIN_KD2;
extern const uint PIN_KD3;
extern const uint PIN_KD4;
extern const uint PIN_KD5;
extern const uint PIN_KD6;
extern const uint PIN_KD7;
extern const uint PIN_KD8;
extern const uint PIN_KD9;
extern const uint PIN_KD10;
extern const uint PIN_KD11;
extern const uint PIN_KD12;
extern const uint PIN_KD13;
extern const uint PIN_KD14;
extern const uint PIN_KD15;

extern const uint PIN_ADC;

#define NUM_ROW  4
#define NUM_COL 16
#define NUM_KEYS 24

// No matrix key held down
#define MATRIX_KEY_NONE 255

extern const int col[NUM_COL];

void hid_task(void);
int deliver_key(int key_to_deliver);
int deliver_key_done(void);

