////////////////////////////////////////////////////////////////////////////////
//
// LED things
//
// Handles the LED driver
//
// Provides a way to turn LEDs on and off
//
//
////////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include <ctype.h>

#include "i2c.h"
#include "led.h"


RGB_LED_INFO led_info[NUM_LEDS] =
  {

    {{ 6, 3}, { 5, 3}, { 4, 3} },  // 0
    {{ 3, 3}, { 2, 3}, { 1, 3} },  // 1
    {{18, 3}, {17, 3}, {16, 3} },  // 2
    {{15, 3}, {14, 3}, {13, 3} },  // 3
    {{ 6, 1}, { 5, 1}, { 4, 1} },  // 4
    {{ 3, 1}, { 2, 1}, { 1, 1} },  // 5
    {{18, 1}, {17, 1}, {16, 1} },  // 6
    {{15, 1}, {14, 1}, {13, 1} },  // 7
    {{ 6, 2}, { 5, 2}, { 4, 2} },  // 8
    {{ 3, 2}, { 2, 2}, { 1, 2} },  // 9
    {{18, 2}, {17, 2}, {16, 2} },  // 10
    {{15, 2}, {14, 2}, {13, 2} },  // 11
    {{ 6, 4}, { 5, 4}, { 4, 4} },  // 12
    {{ 3, 4}, { 2, 4}, { 1, 4} },  // 13
    {{18, 4}, {17, 4}, {16, 4} },  // 14
    {{15, 4}, {14, 4}, {13, 4} },  // 15
    {{ 9, 2}, { 8, 2}, { 7, 2} },  // 16
    {{ 9, 4}, { 8, 4}, { 7, 4} },  // 17
    {{12, 4}, {11, 4}, {10, 4} },  // 18
    {{12, 2}, {11, 2}, {10, 2} },  // 19
    {{ 9, 1}, { 8, 1}, { 7, 1} },  // 20
    {{ 9, 3}, { 8, 3}, { 7, 3} },  // 21
    {{12, 3}, {11, 3}, {10, 3} },  // 22
    {{12, 1}, {11, 1}, {10, 1} },  // 23
  };


void set_led_rgb(int x, int y, int r, int g, int b)
{
  uint8_t reg;
  uint8_t data;
  int led_no = x;
  
  //printf("\n%s:x:%d y:%d r:%d g:%d b:%d", __FUNCTION__, x, y, r, g, b);
  
  i2c_unlock_il31();
  reg = 0xFD;
  data = 0x00;
  i2c_write_byte(&reg, &data);

  // Write R
  RGB_LED_INFO info = led_info[led_no];
  
  reg = CS_SW_TO_OFFSET(info.r.cs, info.r.sw);
  data = r;
  i2c_write_byte(&reg, &data);
  //printf("\n   R reg:%02X data:%02X", reg, data);
  
  reg = CS_SW_TO_OFFSET(info.g.cs, info.g.sw);
  data = g;
  i2c_write_byte(&reg, &data);
  //printf("\n   G reg:%02X data:%02X", reg, data);
  
  reg = CS_SW_TO_OFFSET(info.b.cs, info.b.sw);
  data = b;
  i2c_write_byte(&reg, &data);
  //printf("\n   B reg:%02X data:%02X", reg, data);
  
}

////////////////////////////////////////////////////////////////////////////////

void set_page0_rgb(int x, int r, int g, int b)
{
  uint8_t reg;
  uint8_t data;
  int led_no = x;
  
  //printf("\n%s:offset:%d r:%d g:%d b:%d", __FUNCTION__, x, r, g, b);
  
  i2c_unlock_il31();
  reg = 0xFD;
  data = 0x00;
  i2c_write_byte(&reg, &data);

  // Write R
  RGB_LED_INFO info = led_info[led_no];

  
  reg = x;
  data = r;
  i2c_write_byte(&reg, &data);
  //printf("\n   R reg:%02X data:%02X", reg, data);
}

////////////////////////////////////////////////////////////////////////////////

void clr_led(void)
{
  for(int i=0; i<NUM_LEDS; i++)
    {
      set_led_rgb(i, 0, 0, 0, 0);
    }
}

////////////////////////////////////////////////////////////////////////////////
