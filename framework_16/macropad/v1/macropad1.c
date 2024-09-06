#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"

////////////////////////////////////////////////////////////////////////////////
//
// GPIOs
//
//
////////////////////////////////////////////////////////////////////////////////

const uint PIN_SDAOUT     = 14;
const uint PIN_LATCHOUT2  = 15;
const uint PIN_I2C_SDA    = 16;
const uint PIN_I2C_SCL    = 17;
const uint PIN_LS_DIR     = 18;
const uint PIN_LATCHIN    = 19;
const uint PIN_SCLKIN     = 20;
const uint PIN_SDAIN      = 21;
const uint PIN_LATCHOUT1  = 22;
const uint PIN_SCLKOUT    = 26;
const uint PIN_VBAT_SW_ON = 27;




////////////////////////////////////////////////////////////////////////////////

void oledmain(void);
void serial_loop(void);

int main() {
  sleep_ms(2000);

  stdio_init_all();
  printf("\nMacropad 1");
  
  // Main loop
  while(1)
    {
      serial_loop();
    }
  
}

