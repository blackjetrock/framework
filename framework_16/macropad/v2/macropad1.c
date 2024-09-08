





#include "bsp/board_api.h"
#include "tusb.h"

#include "usb_descriptors.h"

#include "macropad.h"

////////////////////////////////////////////////////////////////////////////////
//
// GPIOs
//
//
////////////////////////////////////////////////////////////////////////////////

const uint PIN_MUX_ENABLE = 4;
const uint PIN_MUX_A      = 1;
const uint PIN_MUX_B      = 2;
const uint PIN_MUX_C      = 3;

const uint PIN_UNUSED1  = 5;
const uint PIN_UNUSED2  = 6;

const uint PIN_KD0  = 8;
const uint PIN_KD1  = 9;
const uint PIN_KD2  = 10;
const uint PIN_KD3  = 11;
const uint PIN_KD4  = 12;
const uint PIN_KD5  = 13;
const uint PIN_KD6  = 14;
const uint PIN_KD7  = 15;
const uint PIN_KD8  = 21;
const uint PIN_KD9  = 20;
const uint PIN_KD10 = 19;
const uint PIN_KD11 = 18;
const uint PIN_KD12 = 17;
const uint PIN_KD13 = 16;
const uint PIN_KD14 = 23;
const uint PIN_KD15 = 22;

const uint PIN_ADC  = 28;


const int col[NUM_COL] =
  {
    PIN_KD0,
    PIN_KD1,
    PIN_KD2,
    PIN_KD3,
    PIN_KD4,
    PIN_KD5,
    PIN_KD6,
    PIN_KD7,
    PIN_KD8,
    PIN_KD9,
    PIN_KD10,
    PIN_KD11,
    PIN_KD12,
    PIN_KD13,
    PIN_KD14,
    PIN_KD15,
  };

void gpio_set_out(int g)
{
  gpio_init(g);
  gpio_set_dir(g, GPIO_OUT);
  printf("\nSetting gpio %d to output", g);
}

void gpio_set_input(int g)
{
  gpio_init(g);
  gpio_set_dir(g, GPIO_IN);
}

void init_gpios(void)
{
  for(int i=0; i<NUM_COL; i++)
    {
      gpio_set_out(col[i]);
    }

  gpio_set_out(PIN_MUX_ENABLE);
  gpio_put(PIN_MUX_ENABLE, 0);

  gpio_set_out(29);
  gpio_put(29, 1);
  
  gpio_set_out(PIN_MUX_A);
  gpio_set_out(PIN_MUX_B);
  gpio_set_out(PIN_MUX_C);

  gpio_set_input(PIN_UNUSED1);
  gpio_set_input(PIN_UNUSED2);
  
}

////////////////////////////////////////////////////////////////////////////////

void init_i2c(void)
{
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);

}

////////////////////////////////////////////////////////////////////////////////

void oledmain(void);
void serial_loop(void);

int main() {



  
  stdio_init_all();
  sleep_ms(2000);
  
  adc_init();
  adc_gpio_init(PIN_ADC);
  adc_select_input(2);
  gpio_pull_up(PIN_ADC);
  init_gpios();

  
  printf("\nMacropad CDC Stdio");

  init_usb();
    
  // Main loop
  while(1)
    {

      tud_task();
      serial_loop();
      //hid_task();
    }
  
}

